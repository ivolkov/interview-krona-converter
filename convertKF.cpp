#pragma hdrstop

#include "convertKF.h"
#include <time.h>

#pragma package(smart_init)

#define errMsg "���������� ������� ���� channels.dat"

CHANNELS * ch ;
FILE * channelsFile ;
FILE * infFile ;
FILE * dataFile ;
FILE * convFile ;

AnsiString filesPath = "" ;
AnsiString convFilePath = "" ;

byte chCnt = 0 ;
int dataCnt = 0 ;

time_t hdrTime ;
char hdrScheme ;
char hdrGageName[20] ;

void saveHeader()
{
	AnsiString stmp = "" ;

	fputs( "KronaConverted\n", convFile ) ;										// 0 ������������� �����
	fputs( "2\n", convFile ) ;													// 1 ������ ���������
	fputs( "\n", convFile ) ;													// 2 ������������� �������
	fputs( "Krona-517\n", convFile ) ;											// 3 ������������ �������
	fputs( "\n", convFile ) ;													// 4 ������ �������� ����
	fputs( "\n", convFile ) ;													// 5 ������ �������� ����������
	fputs( "\n", convFile ) ;													// 6 ����������� ����

	stmp = FloatToStr(1000000 / ch[0].chanData.discretnost) + '\n' ;
	fputs( stmp.c_str(), convFile ) ;											// 7 ������� ������

	stmp = FloatToStr( ch[0].chanData.discretnost * 0.000001 ) + '\n' ;
	fputs( stmp.c_str(), convFile ) ;											// 8 ������ ����� �������

	try {
		strftime( stmp.c_str(), 20, "%d.%m.%Y %H:%M:%S", localtime(&hdrTime) ) ;
		fputs( stmp.c_str(), convFile ) ; 										// 9 ����.�����
	} catch(...) {
	}
	fputc( '\n', convFile ) ; 

	stmp = "" ;
	for ( int i = 0 ; i < chCnt ; i++ ) {
		stmp += ch[i].strName ;
		stmp += "," ;
	}
	stmp[stmp.Length()] = '\n' ;			// ������ ��������� ������� �� \n
	fputs( stmp.c_str(), convFile ) ; 											// 10 ������ �������

	stmp = "" ;
	for ( int i = 0 ; i < chCnt ; i++ ) {
		stmp += "float32," ;
	}
	stmp[stmp.Length()-1] = '\n' ;
	fputs( stmp.c_str(), convFile ) ; 											// 11 ������ ������
	
	fputs( "\n", convFile ) ;													// 12 �������������� ������������
	fputs( "\n", convFile ) ;													// 13 ���
	fputs( "\n", convFile ) ;													// 14 �����������

	fputs( &hdrScheme, convFile ) ;												// 15 ����� ����������
	fputc( '\n', convFile ) ;
	
	fputs( "\n", convFile ) ;													// 16 ������ ��������
	fputs( "\n", convFile ) ;													// 17 ��� ���������
	fputs( "\n", convFile ) ;													// 18 ��������� �������
	fputs( "\n", convFile ) ;													// 19 ��������� �����
	fputs( "\n", convFile ) ;													// 20 ����������� ���
	fputs( "\n", convFile ) ;													// 21 ��
	fputs( "\n", convFile ) ;													// 22 ��
	fputs( "\n", convFile ) ;													// 23 ������������ ������
	fputs( "\n", convFile ) ;													// 24 ����� ������������
	fputs( "\n", convFile ) ;													// 25 ����������

	stmp = IntToStr( dataCnt ) ;
	fputs( stmp.c_str(), convFile ) ;											// 26 ���������� ����� �� �����
	fputs( "\n", convFile ) ;
}


void tryread(void * __prt, unsigned int __size, unsigned int __n, FILE * __stream)
{
	if (!fread(__prt, __size, __n, __stream)) {
		throw Exception( errMsg ) ;
	}
}


bool convertKronaFile(char * fname)
{
	try {
			// �������� ����� channels.dat (������ ���������� � �������)
		channelsFile = fopen( fname, "rb" ) ;
		if ( !channelsFile ) {
			throw Exception( "���������� ������� ���� channels.dat" ) ;
		}

		filesPath = ExtractFilePath( fname ) ;
		convFilePath = filesPath + "convtmp.sdf" ;

		infFile = fopen( AnsiString(filesPath+"data.inf").c_str(), "rb" ) ;
		DATAITEM inf ;
		if ( infFile ) {
			fseek( infFile, sizeof(DWORD), SEEK_CUR ) ;
			fread( &hdrTime, sizeof(time_t), 1, infFile ) ;		// ����� ����������
			fread( &inf, sizeof(DATAITEM), 1, infFile ) ;		// ���������� � ��������
			fseek( infFile, 50, SEEK_CUR ) ;
			fread( &hdrScheme, 1, 1, infFile ) ;				// ����� ���������: 0-������; 1-�����������
			// ... � ����� �������� ��� 4 �������������� ���������
			fclose( infFile ) ; 
		} else {
			hdrTime = 0 ;
			hdrScheme = 0 ;
			hdrGageName[0] = '\0' ;
		}

			// �������� ���������� ����� ��� �����������
		convFile = fopen( convFilePath.c_str(), "wb" ) ;
		if ( !convFile ) {
			throw Exception( "���������� ������� ��������� ���� ��� ������" ) ;
		}
			// ���������� ���������� �������
		tryread( &chCnt, 1, 1, channelsFile ) ;
		if ( chCnt == 0 ) {
			throw Exception( errMsg ) ;
		}

		ch = new CHANNELS[chCnt] ;

			// ���������� ���������� � ���� �������
		for ( int i = 0 ; i < chCnt ; i++ ) {
				// ChanData
			tryread( &ch[i].chanData, sizeof(CHANDATA), 1, channelsFile ) ;
			if ( ch[i].chanData.discretnost == 0 ) {
				ch[i].chanData.discretnost = 100 ;
			}

				// NameCount
			tryread( &ch[i].bNameCount, sizeof(byte), 1, channelsFile ) ;

				// Name
			ch[i].strName = new char[ ch[i].bNameCount ] ;
			tryread( ch[i].strName, ch[i].bNameCount, 1, channelsFile ) ;
			ch[i].strName[ ch[i].bNameCount ] = '\0' ;

				// bLabelCount
			tryread(&ch[i].bLabelCount, sizeof(byte), 1, channelsFile) ;
			
				// lbLabel
			ch[i].lbLabel = new LABELINFO[ ch[i].bLabelCount ] ;
			for ( int j = 0 ; j < ch[i].bLabelCount ; j++ ) {
				tryread( &ch[i].lbLabel[j], sizeof(LABELINFO), 1, channelsFile ) ;
			}
		}

		saveHeader() ; 

		AnsiString dataFilename = "" ;
			// ���������� ���������� ����� � ������
		int * dataCnt = new int[chCnt] ;

		for ( int i = 0 ; i < chCnt ; i ++ ) {
			dataFilename = ExtractFilePath( fname ) + ch[i].chanData.filename + ".chC" ;
			dataFile = fopen( dataFilename.c_str(), "rb" ) ;
			if ( !dataFile ) {
				throw Exception( "���������� ������� ���� " + dataFilename ) ;			// ���������� ������� ����
			}
			fseek( dataFile, 0, SEEK_END ) ;
			dataCnt[i] = ftell(dataFile)/4 ;
			fclose( dataFile ) ;
		}

			// ����������� ����������� ���������� ���������� �����
		int dataCntMax = 0 ;
		for ( int i = 0 ; i < chCnt ; i++ ) {
			if ( dataCnt[i] > dataCntMax ) {
				dataCntMax = dataCnt[i] ;
			}
		}

			// ��������� ������ ������ (*.chC)
		for ( int i = 0 ; i < chCnt ; i ++ ) {
			dataFilename = ExtractFilePath( fname ) + ch[i].chanData.filename + ".chC" ;
			dataFile = fopen( dataFilename.c_str(), "rb" ) ;

				// ���������� ����� ������
			float dataY ;
			float dataX = 0.0 ;
			for ( int j = 0 ; j < dataCnt[i] ; j++ ) {
				dataX += ( ch[i].chanData.discretnost * 0.000001 ) ;
				if ( !fread(&dataY, 4, 1, dataFile) ) {
					break ;
				}
				fwrite( &dataY, 4, 1, convFile ) ;
			}

				// ������ ������������ ����� ������
			for ( int j = dataCnt[i] ; j < dataCntMax ; j++ ) {
				float tmp = 0.0 ;
				fwrite( &tmp, 4, 1, convFile ) ;
			}

			fclose( dataFile ) ;
		}

	} catch( Exception &exception ) {
		//cout << exception.Message.c_str() ;
		delete [] ch ;
		fclose( convFile ) ;
		fclose( channelsFile ) ;
		fclose( dataFile ) ;
		return false ;
	}

	fclose( convFile ) ;
	fclose( channelsFile ) ;
	return true ;
}
