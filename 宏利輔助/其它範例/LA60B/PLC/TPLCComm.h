//---------------------------------------------------------------------------

#ifndef TPLCCommH
#define TPLCCommH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "Comm.h"
#include "TPLC.h"
//---------------------------------------------------------------------------

#define DEFAULT_POLLING_INTERVAL			10		// ms
#define DEFAULT_SEND_INTERVAL				10		// ms
#define DEFAULT_PLC_RESPONSE_TIMEOUT		300		// ms
#define DEFAULT_COMPORT						"Com1"


#define COMM_MAX_LENGTH				4096
#define PLC_MAX_TIMEOUT_COUNT		3
#define PLC_MAX_ERROR_COUNT			3

typedef void __fastcall PACKAGE (__closure *TAfterReceive)(TObject *Sender,unsigned char CPU_ID,RxState State);
typedef void __fastcall PACKAGE (__closure *TAfterTimeout)(TObject *Sender,unsigned char CPU_ID);
typedef void __fastcall PACKAGE (__closure *TAfterSend)(TObject *Sender,unsigned char CPU_ID);

//---------------------------------------------------------------------------
class TPLCComm : public TPLC
{
private:
	TComponent *MyOwner;
	bool __fastcall CreateComponent(void);
	void __fastcall DeleteComponent(void);

	bool pEnabled;
	void __fastcall SetEnabled(bool IsEnable);

	TTimer *Polling;
	void __fastcall PollingTimer(TObject *Sender);
	unsigned int __fastcall GetPollingInterval(void);
	void __fastcall SetPollingInterval(unsigned int Interval);
	unsigned int LastSentTick;
	unsigned int SendGapTick;
	bool IsWaitResponse;
	void __fastcall GetNextTxData(void);
	void __fastcall ReSendTxData(void);

	TComm *pComPort;
	void __fastcall CommRxChar(TObject *Sender, DWORD Count);
	void __fastcall CommWrite(void);
	void __fastcall SetComPort(TComm *Comm);

	TAfterReceive FOnAfterReceive;
	TAfterTimeout FOnAfterTimeout;
	TAfterSend FOnAfterSend;

	int pBaudRate;
	int pDataBits;
	String pParity;
	double pStopBits;
	void __fastcall SetBaudRate(int Rate);
	void __fastcall SetDataBits(int DataBits);
	void __fastcall SetParity(String Parity);
	void __fastcall SetStopBits(double StopBits);

protected:

	unsigned char TxBuffer[COMM_MAX_LENGTH];
	int Tx_Data_Length;
	bool IsResent;
	unsigned char RxBuffer[COMM_MAX_LENGTH];
	int Rx_Data_Length;
	int Rx_Idx;
	RxState LastRxState;

public:
	__fastcall TPLCComm(TComponent* Owner);
	__fastcall ~TPLCComm();

	__property unsigned int PollingInterval	= { read=GetPollingInterval	,write=SetPollingInterval	};
	__property bool Enabled					= { read=pEnabled			,write=SetEnabled		 	};

	__property TComm *ComPort				= { read=pComPort			,write=SetComPort 			};

	__property int BaudRate		= {	read = pBaudRate	,write = SetBaudRate	};
	__property int DataBits		= {	read = pDataBits	,write = SetDataBits	};
	__property String Parity	= {	read = pParity		,write = SetParity		};
	__property double StopBits	= {	read = pStopBits	,write = SetStopBits	};

	__property TAfterReceive OnAfterReceive	= { read=FOnAfterReceive	,write=FOnAfterReceive		};
	__property TAfterTimeout OnAfterTimeout	= { read=FOnAfterTimeout	,write=FOnAfterTimeout		};
	__property TAfterSend OnAfterSend		= { read=FOnAfterSend		,write=FOnAfterSend			};
};
//---------------------------------------------------------------------------
#endif
