#ifndef convertKFH
#define convertKFH

#include <SysUtils.hpp>
#include <stdio.h>

struct CHANDATA {
	short vers ;					// ������ �����, ������ ���� 2
	unsigned int average ;			// ���������� ����������� �������� ��� ���������
	float maxValue ;				// ���������
	char filename[30] ;				// ��� ����� � ������� (0..N,chC)
	char sourceFeature[3] ;			// ��������� ����������
	float toler_AP ;				// ������� �� ��������
	float toler_AM ;
	float toler_TP ;				// ������� �� �������
	float toler_TM ;
	byte type ;						// ��� ��������
	int discretnost ;				// ������������ (� ���; ���� 0, �� ������������ 100 ���)
	char reserv[10] ;				// ���������������
} ;

struct LABELINFO {
	unsigned long dwTime ;			// � ������ ������� ��������� �����
	float flAmpl ;					// � ����� ��������� ��������� �����
	int cx, cy ;					// �������� �� �����
	RECT Koords ;					// ���������� ������
	byte Len ;						// ����� ������
	char Text[100] ;				// �����
} ;

struct CHANNELS {
	CHANDATA chanData ;				// ������ ��������� CHANDATA
	byte bNameCount ;				// ���������� �������� � �������� ������
	char * strName ;				// �������� ������
	byte bLabelCount ;				// ���������� ��������� �����, ��������������� � �������
	LABELINFO * lbLabel ;			// ������ ��������� LABELINFO
} ;

struct DATAITEM {
		// ��������
	char m_Identification[30];		// ������������� ������������ ������� (������������ ����)
	char m_Designation[20];			// ��������������� ����������� (������������ ����)
	char m_Owner[20];				// ���-�������� ������������
	char m_armName[50];				// ������������
	char m_armDesignation[25];		// �������� �����������
	char m_armDocument[25];			// �������� ����������� ���������. ���������
	char m_armLabel[16];			// ����������
	char m_Designer[25];			// �����������
	char m_armFactory[25];			// �����-������������
	byte m_armProduceDay;			// ���� ������������ (�� ������������)
	byte m_armProduceMonth;			// ����� ������������
	int  m_armProduceYear;			// ��� ������������
	char m_armNumber[16];			// ��������� �����
	byte m_armWorkDay;				// ���� ������ ������������
	byte m_armWorkMonth;			// ����� ������ ������������
	byte m_armWorkYear;				// ��� ������ ������������
	byte m_armVypuskDay;			// ���� ������� (�� ������������)
	byte m_armVypuskMonth;			// ����� �������
	int  m_armVypuskYear;			// ��� �������
	char reserv[10];				// (�� ������������)

	char m_armPass[20];				// ������ ��������
	double m_armPressure;			// ������� ��������
	double m_armTemperature;		// ����������� ������� �����
	char m_armEnviremont[21];		// ������� �����
	char m_armExecution[21];		// ����������
	char m_armMaterial[30];			// �������� �������
	char m_armManage[21];			// ������ ����������
	char m_armTypePrivod[30];		// ��� ������� � ��������
	double m_armRollClose;			// �������� ������ "�������"
	double m_armRollOpen;			// �������� ������ "�������"
	double m_armRollZatvor;			// �������� ������ �� �������
	double m_armCountRoll;			// ���������� �������� �� ������� ��������
	double m_armTimeOpen;			// ����� �������� � ��������
	char m_armClassZatvor[16];		// ����� ������������� ������� �� ���� 9544-75
	double m_armHydrResist;			// ����������� ��������������� �������������
	char m_armGroup[26];			// ����� � ������ ��������
	double m_armMass;				// �����
	float m_armTimeSryv;			// ����� ����� (����) (��)
	float m_armTimeUplot;			// ����� ���������� (��)
	char reserv2[2];				// (�� ������������)

		// ������
	char m_prName[21];				// ������������ �������
	char m_prDesignation[25];		// �������� �����������
	char m_prDocument[25];			// ����������� ��������� ���������������� ���������
	char m_prCode[16];				// ��� ���
	char m_prLabel[16];				// ���������� �� �������
	char m_prFactory[25];			// �����-������������
	byte m_prProduceDay;			// ���� ������������ (�� ������������)
	byte m_prProduceMonth;			// ����� ������������
	int m_prProduceYear;			// ��� ������������
	char m_prNumber[16];			// ��������� �����
	byte m_prWorkDay;				// ���� ������ ������������
	byte m_prWorkMonth;				// ����� ������ ������������
	int m_prWorkYear;				// ��� ������ ������������
	byte m_prVypuskDay;				// ���� ������� (�� ������������)
	byte m_prVypuskMonth;			// ����� �������
	int m_prVypuskYear;				// ��� �������
	char reserv3[10];				// (�� ������������)

	char m_prType[21];				// ��� ��������������
	double m_prPower;				// �������� �������������� (���)
	char m_prTypeDv[21];			// ��� ����������������
	char m_prRollClose;				// �������� ������ �� �������� ���� ("�������")
	double m_prRollOpen;			// �������� ������ �� �������� ���� ("�������")
	double m_prKPD;					// ��� ��������� (%)
	double m_prCountRoll;			// ���������� �������� ��������� ����
	char m_prProducer[25];			// ������������
	double m_prTransfer;			// ������������ ��������� ���������
	char m_prPlace[25];				// ����� ��������
	float m_prPWork;				// �����. � �������� ���� (���)
	char reserv4[3];				// (�� ������������)

		// ����������������
	char m_dvName[21];				// ������������ ����������������
	char m_dvFactory[25];			// �����-������������
	byte m_dvProduceDat;			// ���� ������������ (�� ������������)
	byte m_dvProduceMonth;			// ����� ������������
	int m_dvProduceYear;			// ��� ������������
	char m_dvNumber[16];			// ��������� �����
	byte m_dvWorkDay;				// ���� ������ ������������
	byte m_dvWorkMonth;				// ����� ������ ������������
	int m_dvWorkYear;				// ��� ������ ������������
	byte m_dvVypuskDay;				// ���� ������� (�� ������������)
	byte m_dvBypuskMonth;			// ����� �������
	int m_dvVypuskYear;				// ��� �������
	char reserv5[10];				// (�� ������������)

	char m_dvType[21];				// ��� ����������������
	char m_dvTypeBearn1[21];		// ��� ���������� �1
	char m_dvTypeBearn2[21];		// ��� ���������� �2
	double m_dvFreq;				// ������� �������� (��/���)
	double m_dvCurrent;				// ������� ��� (�)
	char m_dvCurPow[15];			// ����������� ���/��������
	double m_dvCurrDif;				// ��������� ������� ���� � ������������
	double m_dvResist;				// ������������� �������� (���)
	double m_dvCosFi;				// Cos �
	double m_dvKPD;					// ���
	char reserv6[10];				// (�� ������������)

		// ������� �������
	char m_hiCicle[26];				// ��������� ����
	int m_hiCount;					// ����� �������� � ���������
	byte m_hiLastDay;				// ���� ���������� �������
	byte m_hiLastMonth;				// ����� ���������� �������
	int m_hiLastYear;				// ��� ���������� �������
	char reserv7[10];				// (�� ������������)
} ;

bool convertKronaFile(char * fname);
#endif
