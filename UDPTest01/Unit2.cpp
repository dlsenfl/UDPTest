//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
struct TstHeader                      // 헤더
{
	BYTE bySTX1;
	BYTE bySTX2;
	WORD wVMSID;
	BYTE byOpCode;
	BYTE bySFNo;
	BYTE byAFNo;
	WORD wLength;
};

struct TstTail                        // 테일
{
	BYTE byETX1;
	BYTE byETX2;
};


struct TstCode01                      // 데이터
{
	BYTE byTemp1;
	BYTE byTemp2;
	BYTE byTemp3;
};

struct TstCode02
{
	WORD  wTemp;
	DWORD dwTemp;
};

struct TstCode03
{
	BYTE byTemp;
	WORD  wTemp;

};



//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TForm2::receiveClick(TObject *Sender)
{
	BYTE byBuf[17];
	ZeroMemory(byBuf, sizeof(byBuf));
	byBuf[0] = 0x10;
	byBuf[1] = 0x02;
	byBuf[2] = 0x05;
	byBuf[3] = 0x00;
	byBuf[4] = 0x02;
	byBuf[5] = 0x01;
	byBuf[6] = 0x01;
	byBuf[7] = 0x06;
	byBuf[8] = 0x00;

//	byBuf[ 9] = 0x01;
//	byBuf[10] = 0x02;
//	byBuf[11] = 0x03;

	byBuf[ 9] = 0x0A;
	byBuf[10] = 0x00;
	byBuf[11] = 0x0F;
	byBuf[12] = 0x00;
	byBuf[13] = 0x00;
	byBuf[14] = 0x00;

	byBuf[15] = 0x10;
	byBuf[16] = 0x03;


	TstHeader stHeader;
	TstTail stTail;
	TstCode01 stCode01;
	TstCode02 stCode02;
	int iIndex;

	ZeroMemory(&stHeader, sizeof(stHeader));           // stHeader의 주소값 위치에서부터  sizeof(stHeader)크기만큼 메모리초기화
	CopyMemory(&stHeader, byBuf, sizeof(stHeader));    // stHeader의 주소값 위치에서부터  byBuf위치에서 시작하는 값을 sizeof(stHeader)크기만큼 복사



	if(stHeader.byOpCode == 0x01 ){
		if(stHeader.wLength > 0){
			if(stHeader.wLength != sizeof(stCode01)) return;
			iIndex = sizeof(stHeader);
			ZeroMemory(&stCode01, sizeof(stCode01));
			CopyMemory(&stCode01, byBuf+iIndex, sizeof(stCode01));
		}else{
			// 요청
			TByteDynArray aBuf;
			iIndex = sizeof(stHeader)+sizeof(stCode01)+sizeof(stTail);
			CopyMemory(&aBuf[0], byBuf, iIndex);
			aBuf.Length = iIndex;
			IdUDPServer1->SendBuffer("127.0.0.1", 50000, aBuf);

		}



		iIndex += sizeof(stCode01);


	}else if(stHeader.byOpCode == 0x02){
		if(stHeader.wLength > 0){
			if(stHeader.wLength != sizeof(stCode02)) return;
			ZeroMemory(&stCode02, sizeof(stCode02));
			CopyMemory(&stCode02, byBuf+iIndex, sizeof(stCode02));
		}else{
        	//요청
		}
//		stCode02.wTemp;
//		stCode02.dwTemp;


		iIndex += sizeof(stCode02);
	}else if(stHeader.byOpCode == 0x03){
	}else if(stHeader.byOpCode == 0x04){
	}else if(stHeader.byOpCode == 0x05){
	}else if(stHeader.byOpCode == 0x06){
	}else if(stHeader.byOpCode == 0x07){
	}else if(stHeader.byOpCode == 0x08){
	}else if(stHeader.byOpCode == 0x09){
	}
	ZeroMemory(&stTail, sizeof(stTail));
	CopyMemory(&stTail, byBuf+iIndex, sizeof(stTail));

	iIndex += sizeof(stTail);

	if(iIndex != (sizeof(stHeader) + stHeader.wLength + sizeof(stTail))){

	}

}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button2Click(TObject *Sender)
{
	TstHeader stHeader;
	TstTail stTail;
	TstCode01 stCode01;
	TstCode02 stCode02;
	int iIndex;

	ZeroMemory(&stHeader, sizeof(stHeader));

	stHeader.bySTX1 = 0x10;
	stHeader.bySTX2 = 0x02;

	stHeader.byOpCode = 0x06;
	stHeader.wLength = 0x0F;


	ZeroMemory(&stCode01, sizeof(stCode01));
	stCode01.byTemp1;
	stCode01.byTemp2;
	stCode01.byTemp3;

	ZeroMemory(&stTail, sizeof(stTail));
	stTail.byETX1;
	stTail.byETX2;



//   BYTE byBuf[1024];
//   Zero
//   Copy(byBuf, &stHeader, sizeof(stHeader));
//   Copy(byBuf+sizeof(stHeader), &stCode01, sizeof(stCode01));
//   Copy(byBuf+sizeof(stHeader)+sizeof(stCode01), &stTail, sizeof(stTail));
//   Server->Send("",5001, byBuf index);


}
//---------------------------------------------------------------------------
void __fastcall TForm2::btStartClick(TObject *Sender)
{
	if(!IdUDPServer1->Active){
		IdUDPServer1->Active = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm2::btSendClick(TObject *Sender)
{
	TstHeader stHeader;
	TstTail stTail;


	int iIndex;

	ZeroMemory(&stHeader, sizeof(stHeader));
												   //초기화 세팅
	stHeader.bySTX1 = 0x10;
	stHeader.bySTX2 = 0x02;

	stHeader.byOpCode = 0x02;
	stHeader.wLength = 0x00;

	ZeroMemory(&stTail, sizeof(stTail));
	stTail.byETX1 = 0x10;
	stTail.byETX2 = 0x03;

	BYTE byBuf[1024];
	ZeroMemory(byBuf, sizeof(byBuf));
	CopyMemory(byBuf, &stHeader, sizeof(stHeader));
	iIndex = sizeof(stHeader);
	CopyMemory(byBuf+iIndex, &stTail, sizeof(stTail));
	iIndex = sizeof(stTail);



	TByteDynArray aBuf;
	aBuf.Length = iIndex;
	CopyMemory(&(aBuf[0]), byBuf, iIndex);

	IdUDPServer1->SendBuffer("127.0.0.1", 50000, aBuf);      // ip, port, data
}
//---------------------------------------------------------------------------

void __fastcall TForm2::IdUDPServer1UDPRead(TIdUDPListenerThread *AThread, TBytes AData,
		  TIdSocketHandle *ABinding)
{
	BYTE byBuf[1024];
	ZeroMemory(byBuf, sizeof(byBuf));

	BytesToRaw(AData, byBuf, AData.Length);

	TstHeader stHeader;
	TstTail stTail;
	TstCode01 stCode01;
	TstCode02 stCode02;
	int iIndex;

	ZeroMemory(&stHeader, sizeof(stHeader));
	CopyMemory(&stHeader, byBuf, sizeof(stHeader));
	iIndex = sizeof(stHeader);


	if		(stHeader.byOpCode == 0x01 ){
		if(stHeader.wLength > 0){
			if(stHeader.wLength != sizeof(stCode01)) return;
			ZeroMemory(&stCode01, sizeof(stCode01));
			CopyMemory(&stCode01, byBuf+iIndex, sizeof(stCode01));
		}else{
			// 요청
		}



		iIndex += sizeof(stCode01);


	}else if(stHeader.byOpCode == 0x02){
		if(stHeader.wLength > 0){

			if(stHeader.wLength != sizeof(stCode02)) return;
			ZeroMemory(&stCode02, sizeof(stCode02));
			CopyMemory(&stCode02, byBuf+iIndex, sizeof(stCode02));

			stCode02.wTemp;
			stCode02.dwTemp;
			iIndex += sizeof(stCode02);
		}else{
			ShowMessage("요청이네....");
		}
	}else if(stHeader.byOpCode == 0x03){
	}else if(stHeader.byOpCode == 0x04){
	}else if(stHeader.byOpCode == 0x05){
	}else if(stHeader.byOpCode == 0x06){
	}else if(stHeader.byOpCode == 0x07){
	}else if(stHeader.byOpCode == 0x08){
	}else if(stHeader.byOpCode == 0x09){
	}
	ZeroMemory(&stTail, sizeof(stTail));
	CopyMemory(&stTail, byBuf+iIndex, sizeof(stTail));

	iIndex += sizeof(stTail);

	if(iIndex != (sizeof(stHeader) + stHeader.wLength + sizeof(stTail))){

	}



}
//---------------------------------------------------------------------------
