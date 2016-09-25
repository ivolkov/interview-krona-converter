#ifndef convertKFH
#define convertKFH

#include <SysUtils.hpp>
#include <stdio.h>

struct CHANDATA {
	short vers ;					// версия файла, должна быть 2
	unsigned int average ;			// количество усредняемых значений при отрисовке
	float maxValue ;				// амплитуда
	char filename[30] ;				// имя файла с данными (0..N,chC)
	char sourceFeature[3] ;			// служебная информация
	float toler_AP ;				// допуски по арматуре
	float toler_AM ;
	float toler_TP ;				// допуски по времени
	float toler_TM ;
	byte type ;						// тип адаптера
	int discretnost ;				// дискретность (в мкс; если 0, то используется 100 мкс)
	char reserv[10] ;				// зарезервировано
} ;

struct LABELINFO {
	unsigned long dwTime ;			// к какому времени привязана метка
	float flAmpl ;					// к какой амплитуде привязана метка
	int cx, cy ;					// смещение от точки
	RECT Koords ;					// координаты текста
	byte Len ;						// длина текста
	char Text[100] ;				// текст
} ;

struct CHANNELS {
	CHANDATA chanData ;				// массив элементов CHANDATA
	byte bNameCount ;				// количество символов в названии канала
	char * strName ;				// название канала
	byte bLabelCount ;				// количество плавающих меток, ассоциированных с каналом
	LABELINFO * lbLabel ;			// массив элементов LABELINFO
} ;

struct DATAITEM {
		// арматура
	char m_Identification[30];		// идентификация технического объекта (обязательное поле)
	char m_Designation[20];			// технологическое обозначение (обязательное поле)
	char m_Owner[20];				// цех-владелец оборудования
	char m_armName[50];				// наименование
	char m_armDesignation[25];		// условное обозначение
	char m_armDocument[25];			// условное обозначение конструкт. документа
	char m_armLabel[16];			// маркировка
	char m_Designer[25];			// разработчик
	char m_armFactory[25];			// завод-изготовитель
	byte m_armProduceDay;			// день изготовления (не используется)
	byte m_armProduceMonth;			// месяц изготовления
	int  m_armProduceYear;			// год изготовления
	char m_armNumber[16];			// заводской номер
	byte m_armWorkDay;				// день начала эксплуатации
	byte m_armWorkMonth;			// месяц начала эксплуатации
	byte m_armWorkYear;				// год начала эксплуатации
	byte m_armVypuskDay;			// день выпуска (не используется)
	byte m_armVypuskMonth;			// месяц выпуска
	int  m_armVypuskYear;			// год выпуска
	char reserv[10];				// (не используется)

	char m_armPass[20];				// проход условный
	double m_armPressure;			// рабочее давление
	double m_armTemperature;		// температура рабочей среды
	char m_armEnviremont[21];		// рабочая среда
	char m_armExecution[21];		// исполнение
	char m_armMaterial[30];			// материал корпуса
	char m_armManage[21];			// способ управления
	char m_armTypePrivod[30];		// тип привода и мощность
	double m_armRollClose;			// крутящий момент "Закрыто"
	double m_armRollOpen;			// крутящий момент "Открыто"
	double m_armRollZatvor;			// крутящий момент на затворе
	double m_armCountRoll;			// количество оборотов до полного закрытия
	double m_armTimeOpen;			// время открытия и закрытия
	char m_armClassZatvor[16];		// класс герметичности затвора по ГОСТ 9544-75
	double m_armHydrResist;			// коэффициент гидравлического сопротивления
	char m_armGroup[26];			// класс и группа арматуры
	double m_armMass;				// масса
	float m_armTimeSryv;			// время срыва (люфт) (мс)
	float m_armTimeUplot;			// время уплотнения (мс)
	char reserv2[2];				// (не используется)

		// привод
	char m_prName[21];				// наименование привода
	char m_prDesignation[25];		// условное обозначение
	char m_prDocument[25];			// обозначение основного конструкторского документа
	char m_prCode[16];				// код ОКП
	char m_prLabel[16];				// маркировка на корпусе
	char m_prFactory[25];			// завод-изготовитель
	byte m_prProduceDay;			// день изготовления (не используется)
	byte m_prProduceMonth;			// месяц изготовления
	int m_prProduceYear;			// год изготовления
	char m_prNumber[16];			// заводской номер
	byte m_prWorkDay;				// день начала эксплуатации
	byte m_prWorkMonth;				// месяц начала эксплуатации
	int m_prWorkYear;				// год начала эксплуатации
	byte m_prVypuskDay;				// день выпуска (не используется)
	byte m_prVypuskMonth;			// месяц выпуска
	int m_prVypuskYear;				// год выпуска
	char reserv3[10];				// (не используется)

	char m_prType[21];				// тип электропривода
	double m_prPower;				// мощность электропривода (КВт)
	char m_prTypeDv[21];			// тип электродвигателя
	char m_prRollClose;				// крутящий момент на выходном валу ("Закрыто")
	double m_prRollOpen;			// крутящий момент на выходном валу ("Открыто")
	double m_prKPD;					// КПД редуктора (%)
	double m_prCountRoll;			// количество оборотов выходного вала
	char m_prProducer[25];			// изготовитель
	double m_prTransfer;			// передаточное отношение редуктора
	char m_prPlace[25];				// место распитки
	float m_prPWork;				// актив. Р рабочего хода (КВт)
	char reserv4[3];				// (не используется)

		// электродвигатель
	char m_dvName[21];				// наименование электродвигателя
	char m_dvFactory[25];			// завод-изготовитель
	byte m_dvProduceDat;			// день изготовления (не используется)
	byte m_dvProduceMonth;			// месяц изготовления
	int m_dvProduceYear;			// год изготовления
	char m_dvNumber[16];			// заводской номер
	byte m_dvWorkDay;				// день начала эксплуатации
	byte m_dvWorkMonth;				// месяц начала эксплуатации
	int m_dvWorkYear;				// год начала эксплуатации
	byte m_dvVypuskDay;				// день выпуска (не используется)
	byte m_dvBypuskMonth;			// месяц выпуска
	int m_dvVypuskYear;				// год выпуска
	char reserv5[10];				// (не используется)

	char m_dvType[21];				// тип электродвигателя
	char m_dvTypeBearn1[21];		// тип подшипника №1
	char m_dvTypeBearn2[21];		// тип подшипника №2
	double m_dvFreq;				// частота вращения (об/мин)
	double m_dvCurrent;				// пускойо ток (А)
	char m_dvCurPow[15];			// номинальный ток/мощность
	double m_dvCurrDif;				// отношение пуского тока к номинальному
	double m_dvResist;				// сопротивление изоляции (МОм)
	double m_dvCosFi;				// Cos ф
	double m_dvKPD;					// КПД
	char reserv6[10];				// (не используется)

		// история ремонта
	char m_hiCicle[26];				// ремонтный цикл
	int m_hiCount;					// число ремонтов с разборкой
	byte m_hiLastDay;				// день последнего ремонта
	byte m_hiLastMonth;				// месяц последнего ремонта
	int m_hiLastYear;				// год последнего ремонта
	char reserv7[10];				// (не используется)
} ;

bool convertKronaFile(char * fname);
#endif
