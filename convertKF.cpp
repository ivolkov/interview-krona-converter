#pragma hdrstop

#include "convertKF.h"
#include <time.h>

#pragma package(smart_init)

#define errMsg "Невозможно открыть файл channels.dat"

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

	fputs( "KronaConverted\n", convFile ) ;										// 0 идентификатор файал
	fputs( "2\n", convFile ) ;													// 1 версия заголовка
	fputs( "\n", convFile ) ;													// 2 идентификатор прибора
	fputs( "Krona-517\n", convFile ) ;											// 3 наименование прибора
	fputs( "\n", convFile ) ;													// 4 способ датчиков тока
	fputs( "\n", convFile ) ;													// 5 способ датчиков напряжения
	fputs( "\n", convFile ) ;													// 6 поддиапазон тока

	stmp = FloatToStr(1000000 / ch[0].chanData.discretnost) + '\n' ;
	fputs( stmp.c_str(), convFile ) ;											// 7 частота опроса

	stmp = FloatToStr( ch[0].chanData.discretnost * 0.000001 ) + '\n' ;
	fputs( stmp.c_str(), convFile ) ;											// 8 период между точками

	try {
		strftime( stmp.c_str(), 20, "%d.%m.%Y %H:%M:%S", localtime(&hdrTime) ) ;
		fputs( stmp.c_str(), convFile ) ; 										// 9 дата.время
	} catch(...) {
	}
	fputc( '\n', convFile ) ; 

	stmp = "" ;
	for ( int i = 0 ; i < chCnt ; i++ ) {
		stmp += ch[i].strName ;
		stmp += "," ;
	}
	stmp[stmp.Length()] = '\n' ;			// замена последней запятой на \n
	fputs( stmp.c_str(), convFile ) ; 											// 10 список каналов

	stmp = "" ;
	for ( int i = 0 ; i < chCnt ; i++ ) {
		stmp += "float32," ;
	}
	stmp[stmp.Length()-1] = '\n' ;
	fputs( stmp.c_str(), convFile ) ; 											// 11 формат данных
	
	fputs( "\n", convFile ) ;													// 12 технологичесое наименование
	fputs( "\n", convFile ) ;													// 13 ФИО
	fputs( "\n", convFile ) ;													// 14 направление

	fputs( &hdrScheme, convFile ) ;												// 15 схема управления
	fputc( '\n', convFile ) ;
	
	fputs( "\n", convFile ) ;													// 16 расчёт мощности
	fputs( "\n", convFile ) ;													// 17 тип измерения
	fputs( "\n", convFile ) ;													// 18 состояние системы
	fputs( "\n", convFile ) ;													// 19 пассивный режим
	fputs( "\n", convFile ) ;													// 20 номинальный ток
	fputs( "\n", convFile ) ;													// 21 КЗ
	fputs( "\n", convFile ) ;													// 22 ТЗ
	fputs( "\n", convFile ) ;													// 23 срабатывание защиты
	fputs( "\n", convFile ) ;													// 24 время срабатывания
	fputs( "\n", convFile ) ;													// 25 примечание

	stmp = IntToStr( dataCnt ) ;
	fputs( stmp.c_str(), convFile ) ;											// 26 количество точек на канал
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
			// открытие файла channels.dat (хранит информацию о каналах)
		channelsFile = fopen( fname, "rb" ) ;
		if ( !channelsFile ) {
			throw Exception( "Невозможно открыть файл channels.dat" ) ;
		}

		filesPath = ExtractFilePath( fname ) ;
		convFilePath = filesPath + "convtmp.sdf" ;

		infFile = fopen( AnsiString(filesPath+"data.inf").c_str(), "rb" ) ;
		DATAITEM inf ;
		if ( infFile ) {
			fseek( infFile, sizeof(DWORD), SEEK_CUR ) ;
			fread( &hdrTime, sizeof(time_t), 1, infFile ) ;		// время сохранения
			fread( &inf, sizeof(DATAITEM), 1, infFile ) ;		// информация о задвижке
			fseek( infFile, 50, SEEK_CUR ) ;
			fread( &hdrScheme, 1, 1, infFile ) ;				// схема включения: 0-звезда; 1-треугольник
			// ... в файле хранится еще 4 неиспользуемых параметра
			fclose( infFile ) ; 
		} else {
			hdrTime = 0 ;
			hdrScheme = 0 ;
			hdrGageName[0] = '\0' ;
		}

			// открытие временного файла для конвертации
		convFile = fopen( convFilePath.c_str(), "wb" ) ;
		if ( !convFile ) {
			throw Exception( "Невозможно открыть временный файл для записи" ) ;
		}
			// считывание количества каналов
		tryread( &chCnt, 1, 1, channelsFile ) ;
		if ( chCnt == 0 ) {
			throw Exception( errMsg ) ;
		}

		ch = new CHANNELS[chCnt] ;

			// считывание информации о всех каналах
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
			// вычисление количества точек в файлах
		int * dataCnt = new int[chCnt] ;

		for ( int i = 0 ; i < chCnt ; i ++ ) {
			dataFilename = ExtractFilePath( fname ) + ch[i].chanData.filename + ".chC" ;
			dataFile = fopen( dataFilename.c_str(), "rb" ) ;
			if ( !dataFile ) {
				throw Exception( "Невозможно открыть файл " + dataFilename ) ;			// невозможно открыть файл
			}
			fseek( dataFile, 0, SEEK_END ) ;
			dataCnt[i] = ftell(dataFile)/4 ;
			fclose( dataFile ) ;
		}

			// определение максимально возможного количества точек
		int dataCntMax = 0 ;
		for ( int i = 0 ; i < chCnt ; i++ ) {
			if ( dataCnt[i] > dataCntMax ) {
				dataCntMax = dataCnt[i] ;
			}
		}

			// обработка файлов данных (*.chC)
		for ( int i = 0 ; i < chCnt ; i ++ ) {
			dataFilename = ExtractFilePath( fname ) + ch[i].chanData.filename + ".chC" ;
			dataFile = fopen( dataFilename.c_str(), "rb" ) ;

				// считывание сырых данных
			float dataY ;
			float dataX = 0.0 ;
			for ( int j = 0 ; j < dataCnt[i] ; j++ ) {
				dataX += ( ch[i].chanData.discretnost * 0.000001 ) ;
				if ( !fread(&dataY, 4, 1, dataFile) ) {
					break ;
				}
				fwrite( &dataY, 4, 1, convFile ) ;
			}

				// запись недостающего блока данных
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
