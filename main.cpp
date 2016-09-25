//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "convertKF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "iComponent"
#pragma link "iCustomComponent"
#pragma link "iPlot"
#pragma link "iPlotComponent"
#pragma link "iVCLComponent"
#pragma resource "*.dfm"

#include <queue.h>
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	dlgOpen1 -> InitialDir = ExtractFileDir( Application->ExeName ) ;
	dlgOpen2 -> InitialDir = ExtractFileDir( Application->ExeName ) ;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::dlgOpen1CanClose(TObject *Sender, bool &CanClose)
{
	ShowMessage( BoolToStr(convertKronaFile(dlgOpen1->FileName.c_str()), true) ) ;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	dlgOpen1 -> Execute() ;	
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
	dlgOpen2 -> Execute() ;	
}
//---------------------------------------------------------------------------

bool __fastcall TForm1::OpenKronaFile(char * filename)
{
	FILE * pFile;
	double timeIncrement = 0.0002 ;					// инкремент для оси Х
	long fdivider = 0 ;								// разделитель каналов в файле
	long dataStart = 0 ;							// начало блока данных
	long dataSize = 0 ;								// размер блока данных
	AnsiString header[27] ;							// заголовок файла
	AnsiString * chNames ;							// названия каналов
	int chCnt = 0 ;

	pFile = fopen( filename, "rb" ) ;
	if ( !pFile ) {
		return false;					// невозможно открыть файл
		}

	fseek( pFile, 0, SEEK_END ) ;
	if ( !ftell(pFile) ) {
		fclose( pFile ) ;
		return false ;					// файл пуст
		}

		// чтение заголовка файла в header
	fseek( pFile, 0, SEEK_SET ) ;
	char cData[1024];
	for ( int i = 0 ; i < 27 ; i++ ) {
		fgets( cData, 512, pFile ) ;
		header[i] = cData ;
		header[i].Delete( header[i].Length(), 1 ) ;		// удаление '\n'
	}

	AnsiString header10 = header[10] ;
		// подсчёт количестка каналов по header[9]
	if ( header10.Length() != 0 ) {
		chCnt = 1 ;
	} else {
		fclose( pFile ) ;
		return false ;				// список каналов пуст
	}
	for ( int i = 1 ; i < header10.Length() ; i++ ) {
		if ( header10[i] == ',' ) {
			chCnt++ ;
		}
	}

		// извлечение из header[8] периода между точками
	AnsiString header8 = header[8] ;
	try {
		timeIncrement = StrToFloat( header8 ) ;
	} catch (...) {
		timeIncrement = 0 ;
	}

		// инициализация chNames
	chNames = new AnsiString[chCnt] ;

		// заполнение массива имён каналов
	int sPos, sCnt = 0 ;
	do {
		sPos = header10.Pos( ',' ) ;
		if ( sPos ) {
			chNames[sCnt] = header10.SubString( 0, sPos-1 ) ;
			header10.Delete( 1, sPos ) ;
			sCnt++ ;
		}
	} while ( sPos ) ;
	chNames[chCnt-1] = header10.SubString( 0, header10.Length() ) ;

		// определение начала и размера блока данных
	dataStart = ftell(pFile) ;
	fseek(pFile, 0, SEEK_END) ;
	dataSize = ftell(pFile) - dataStart ;

	if ( dataSize == 0 ) {
		fclose( pFile ) ;
		return false ;					// блок данных пуст
		}

	fdivider = ( (dataSize/chCnt)/4 ) ;

		// удаление существующих каналов
	while ( iPlot1->ChannelCount() ) {
		iPlot1 -> DeleteChannel( 0 ) ;
	}

		// добавление новых каналов
	for ( int i = 0 ; i < chCnt ; i ++ ) {
		iPlot1 -> AddChannel() ;
		iPlot1 -> Channel[i] -> TitleText = chNames[i] ;
	}

		 // возврат к началу блока данных
	fseek( pFile, dataStart, SEEK_SET ) ;

	float fData ;
	float fTime ;

		// заполнение каналов
	for ( int i = 0 ; i < chCnt ; i++ ) {
		fTime = 0.0 ;
		for ( int j = 0 ; j < fdivider ; j++ ) {
			fread( &fData, 4, 1, pFile ) ;
			fTime += timeIncrement ;
			iPlot1 -> Channel[i] -> AddXY( fTime, fData ) ;
		}
	}

	iPlot1 -> XAxis[0] -> Min = 0 ;
	iPlot1 -> XAxis[0] -> Span = iPlot1 -> Channel[0] -> GetXMax() ;

    fclose( pFile ) ;
	return true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::dlgOpen2CanClose(TObject *Sender, bool &CanClose)
{
	ShowMessage( BoolToStr( OpenKronaFile(dlgOpen2->FileName.c_str()), true) ) ;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
	int indx = iPlot1 -> AddAnnotation() ;
	iPlot1 -> Annotation[indx] -> Text = "test" ;
	iPlot1 -> Annotation[indx] -> X = 2.5 ;
	iPlot1 -> Annotation[indx] -> Y = 100.0 ;
	iPlot1 -> Annotation[indx] -> ChannelName = iPlot1 -> Channel[0] -> Name ;
	iPlot1 -> Annotation[indx] -> Reference = iprtChannel ;
}
//---------------------------------------------------------------------------

