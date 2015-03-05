//---------------------------------------------------------------------------


#pragma hdrstop

#include "TPLCComm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Comm"

//---------------------------------------------------------------------------
__fastcall TPLCComm::TPLCComm(TComponent* Owner)
	:TPLC(Owner)
{
	MyOwner		= Owner;

	pEnabled	= false;
	if( !CreateComponent() )
	{
		// can't create component, error process
		;
	}

	memset( TxBuffer, 0, COMM_MAX_LENGTH );
	Tx_Data_Length	= 0;
	IsResent		= 0;
	memset( RxBuffer, 0, COMM_MAX_LENGTH );
	Rx_Data_Length	= 0;
	Rx_Idx			= 0;
	LastRxState		= COMM_WAIT_RESPONSE;

}
//---------------------------------------------------------------------------
__fastcall TPLCComm::~TPLCComm()
{
	DeleteComponent();
}
//---------------------------------------------------------------------------
bool __fastcall TPLCComm::CreateComponent(void)
{
	Polling	= new TTimer( MyOwner );
	if( !Polling )
		return false;
	Polling->Enabled	= false;

	Polling->OnTimer	= PollingTimer;
	Polling->Interval	= DEFAULT_POLLING_INTERVAL;

	LastSentTick	= 0;
	SendGapTick		= 0;
	IsWaitResponse	= false;

	FOnAfterReceive	= NULL;
	FOnAfterTimeout	= NULL;
	FOnAfterSend	= NULL;
	return true;
}
//---------------------------------------------------------------------------
void __fastcall TPLCComm::DeleteComponent(void)
{
	if( Polling )
	{
		delete Polling;
		Polling	= NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TPLCComm::SetComPort(TComm *Comm)
{
	pComPort	= Comm;
	if( !pComPort )
		return;

	pComPort->OnRxChar	= CommRxChar;
}
//---------------------------------------------------------------------------
void __fastcall TPLCComm::SetEnabled(bool IsEnable)
{
	pEnabled	= IsEnable;
	Polling->Enabled	= pEnabled;
}
//---------------------------------------------------------------------------
unsigned int __fastcall TPLCComm::GetPollingInterval(void)
{
	if( !Polling )
		return 0;
	return Polling->Interval;
}
//---------------------------------------------------------------------------
void __fastcall TPLCComm::SetPollingInterval(unsigned int Interval)
{
	if( !Polling )
		return;
	Polling->Interval	= Interval;
}
//---------------------------------------------------------------------------
void __fastcall TPLCComm::PollingTimer(TObject *Sender)
{
	bool next_tx_data = true;

	if( IsWaitResponse )
	{
		if( TimeGoesBy(LastSentTick) < DEFAULT_PLC_RESPONSE_TIMEOUT )
			return;

		if( FOnAfterTimeout )
			FOnAfterTimeout( ((TObject*)this), CommCPU->CPU_ID );

		// if plc timeout, don't get next data. send the latest data again
		if( CommCPU->State != PLC_OFF )
		{
			if( ++CommCPU->TimeoutCount >= PLC_MAX_TIMEOUT_COUNT )
				CommCPU->State	= PLC_OFF;
		}
	}
	else if( TimeGoesBy(SendGapTick) < DEFAULT_SEND_INTERVAL )
	{
		return;
	}

	if( next_tx_data )
	{
		if( CommCPU->State == PLC_OFF )	// PLC off line, don't communicate with same PLC
		{
			GetNextTxData();
		}
		else
		{
#if 1
			GetNextTxData();
#else
			switch( LastRxState )
			{
				case COMM_RX_OK:
					GetNextTxData();
					break;

				/*
				// if Something error must re-send
				// TxBuffer not clear so it can send directly
				*/
				case COMM_WAIT_RESPONSE:				// Receive not complete, maybe timeout
				case COMM_CHECKSUM_ERR:
				case COMM_LENGTH_ERR:
				case COMM_DATA_ERR:
					/*
					if( ++CommCPU->ErrorCount >= PLC_MAX_ERROR_COUNT )
					{
						//
						break;
					}
					*/
				default:
					break;
			}
#endif
		}
	}
	CommWrite();
}
//---------------------------------------------------------------------------
void __fastcall TPLCComm::CommRxChar(TObject *Sender, DWORD Count)
{
	// If received data length over the buffer size
	// reset Rx_Idx and Rx_Data_Length to avoid memory write error
	// after received, data check will occur error.
	// So error process after receive completed
	if( Rx_Idx + Count >= COMM_MAX_LENGTH )
	{
		Rx_Data_Length	= 0;
		Rx_Idx			= 0;
	}
	ComPort->Read( RxBuffer + Rx_Idx, Count );
	Rx_Data_Length	+= 	Count;
	Rx_Idx			=	Rx_Data_Length;

	LastRxState	= CheckRxData( RxBuffer, Rx_Data_Length );
	switch( LastRxState )
	{
		case COMM_WAIT_RESPONSE:
			return;

		case COMM_LENGTH_ERR:	// length error
			CommCPU->State	= PLC_LENGTH_ERR;
			break;

		case COMM_CHECKSUM_ERR:	// checksum error
			CommCPU->State		= PLC_CHECKSUM_ERR;
			CommCPU->ErrorCount	= 0;
			break;

		case COMM_RX_OK:
			CommCPU->State	= PLC_ONLINE;
			break;

		case COMM_DATA_ERR:		// Data content or format error
		default:
			CommCPU->State	= PLC_DATA_ERR;
			break;
	}

	CommCPU->TimeoutCount	= 0;

	// Set Flag
	SendGapTick		= GetTickCount();
	IsWaitResponse	= false;

	if( this->Type == SCALE )
	{
		Rx_Data_Length	= 0;
		Rx_Idx			= 0;
		memset( RxBuffer, 0, COMM_MAX_LENGTH );
	}

	if( OnAfterReceive )
		OnAfterReceive( ((TObject*)this), CommCPU->CPU_ID, LastRxState );
}
//---------------------------------------------------------------------------
void __fastcall TPLCComm::GetNextTxData(void)
{
	memset( TxBuffer, 0, COMM_MAX_LENGTH );
	Tx_Data_Length	= NextTxData( TxBuffer );
}
//---------------------------------------------------------------------------
void __fastcall TPLCComm::ReSendTxData(void)
{
	memset( TxBuffer, 0, COMM_MAX_LENGTH );
	Tx_Data_Length	= ReGetTxData( TxBuffer );
}
//---------------------------------------------------------------------------
//
//	Method : CommWrite
//
//	Note : 	After Comm write data, don't clear TxBuffer
//          If Comm error, TxBuffer has the same datas to re-write
//
//---------------------------------------------------------------------------
void __fastcall TPLCComm::CommWrite(void)
{
	if( Tx_Data_Length <= 0 )
		GetNextTxData();

	if( Tx_Data_Length > 0 )
	{
		// Clear RxBuffer and properties
		memset( RxBuffer, 0, COMM_MAX_LENGTH );
		Rx_Data_Length	= 0;
		Rx_Idx			= 0;
		LastRxState 	= COMM_WAIT_RESPONSE;

		// ComPort write data
		ComPort->Write( TxBuffer, Tx_Data_Length );

		// Set Flag
		LastSentTick	= GetTickCount();
		IsWaitResponse	= true;

		if( OnAfterSend )
			OnAfterSend( ((TObject*)this), CommCPU->CPU_ID );
	}
}
//---------------------------------------------------------------------------
void __fastcall TPLCComm::SetBaudRate(int Rate)
{
	pBaudRate	= Rate;
	switch( pBaudRate )
	{
		case 19200:
			ComPort->BaudRate	= br19200;
			break;

		case 38400:
			ComPort->BaudRate	= br38400;
			break;

		case 57600:
			ComPort->BaudRate	= br57600;
			break;

		case 115200:
			ComPort->BaudRate	= br115200;
			break;

		case 9600:
		default:
			ComPort->BaudRate	= br9600;
			break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TPLCComm::SetDataBits(int DataBits)
{
	pDataBits	= DataBits;
	switch( pDataBits )
	{
		case 4:
			ComPort->DataBits	= da4;
			break;

		case 5:
			ComPort->DataBits	= da5;
			break;

		case 6:
			ComPort->DataBits	= da6;
			break;

		case 7:
			ComPort->DataBits	= da7;
			break;

		case 8:
		default:
			ComPort->DataBits	= da8;
			break;
	}
}
//---------------------------------------------------------------------------
//
//	Function : SetParity
//
//	Note :
//			odd and even are reverse in BCB Comm component
//
//---------------------------------------------------------------------------
void __fastcall TPLCComm::SetParity(String Parity)
{
	pParity	= Parity;
	if( Parity.Trim().UpperCase() == "EVEN" )
	{
		ComPort->Parity	= paOdd;
	}
	else if( Parity.Trim().UpperCase() == "ODD" )
	{
		ComPort->Parity	= paEven;
	}
	else
	{
		ComPort->Parity	= paNone;
	}
}
//---------------------------------------------------------------------------
void __fastcall TPLCComm::SetStopBits(double StopBits)
{
	pStopBits	= StopBits;
	switch( int(pStopBits * 10) )
	{
		case 15:
			ComPort->StopBits	= sb15;
			break;

		case 20:
			ComPort->StopBits	= sb20;
			break;

		case 10:
		default:
			ComPort->StopBits	= sb10;
			break;
	}
}
//---------------------------------------------------------------------------

