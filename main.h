//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include "iComponent.hpp"
#include "iCustomComponent.hpp"
#include "iPlot.hpp"
#include "iPlotComponent.hpp"
#include "iVCLComponent.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TOpenDialog *dlgOpen1;
	TiPlot *iPlot1;
	TPanel *Panel1;
	TButton *Button1;
	TButton *Button2;
	TOpenDialog *dlgOpen2;
	TButton *Button3;
	void __fastcall dlgOpen1CanClose(TObject *Sender, bool &CanClose);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	bool __fastcall OpenKronaFile(char * filename);
	void __fastcall dlgOpen2CanClose(TObject *Sender, bool &CanClose);
	void __fastcall Button3Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
