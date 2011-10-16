//*****************************************************************************
//                     
//		PhiConv
//		(previously known as DifConv)					 
//											 
//		v. 4.5 (MS Windows DOS-9x)	 			 
//                                                          
//		PhiConv converts 'PHI Surface Analysis Software' 
//		proprietary ASCII files into conventional ASCII file
//
//		2003-2010 - Nicola Ferralis <feranick@gmail.com>
//
//		This program (source code and binaries) is free software; 
//		you can redistribute it and/or modify it under the terms of the
//		GNU General Public License as published by the Free Software 
//		Foundation; either version 3 of the License, or any later version.
//
//		This program is distributed in the hope that it will be useful,
//		but WITHOUT ANY WARRANTY; without even the implied warranty of
//		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//		GNU General Public License for more details.
//
//		You can find a complete copy of the GNU General Public License at:
//		http://www.gnu.org/licenses/gpl.txt
//												             
//**********************************************************************************



#if     _MSC_VER
// Use these for MSVC
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
#include <stdio.h>
#include <errno.h>
#include <windows.h>

using namespace std; 

#define popen _popen  
#define MSini 1   // 1: save config file in C:/ (MS32 only)    0: save it in the same folder 
#include "direct.h"		


#else
// Use these for gcc
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <math.h>
#include <stdio.h>  
#include <errno.h>
#define MSini 0
using namespace std; 
#endif

#define GPFIFO "./gpio" 

#define PATH_MAX 200
#define Max 10000
#define CMAXI 200

int operate(char a[15], char b[15]);
int outtype, infofile, flag, vplot;
int times, typeAES;
int typeAES1;
int outtype0=1;
int infofile0=1;
int vplot0=2;

FILE *command, *command2, *gpin;

#if     _MSC_VER
char ini[]="PhiConv.cfg";
#else
char ini[]=".phiconv";
#endif

char version[]= "4.5 - 20100406";

#if     _MSC_VER
	#if MSini ==1					
	char inipath[]="C:/";					
	char mainpath[200];	
	#endif
#else
char mainpath[PATH_MAX+1];
#endif

typedef struct acq{
	char nameorigfile[200];
	char acqtype[10];
	char comment[200];
	double noOfSweep;
	double evPerStep;
	double sweepStart;
	double sweepEnd;
	double timePerStep;
	double range;
	} acq;

typedef struct mousepos{
	double x;
	double y;
	} mouse;

acq a;	
static int steps;
int j, Comp, Comp2, btype;

char datextension[]=".dat";
char difextension[5];
char infoextension[]=".info.txt";
char ascextensionS[]=".asc";
char ascextensionB[]=".ASC";
char difextensionS[]=".dif";
char difextensionB[]=".DIF";

acq extract(char* name, char* nameout, char *nameinfo);
acq extractW32(char* name, char* nameout, char *nameinfo);
void Plot(char* nameout);
void closePlot();
int ReadKey();
float ReadKeyF();
void PreferencePanel();
void SleepP();
//int formatRec(char *name, char nameIN[15], char namenoext[15], const char *fc);
void formatRec(char *nameIN, char *name);	
int single();
int batch();
int DnD(char *name);
int ChemAnal();
void ChemID(double a);
mouse det(int b);
void elcond(double a, double en, const char *el);


int main(int argc, char *argv[])
{	

	#if     _MSC_VER
		#if MSini ==1
		_chdir(_getcwd(mainpath, 200));  
		_chdir(inipath);					
		#endif
	#else
	int rc=0;
	char *pt;	
	pt=getcwd(mainpath, PATH_MAX+1);	
	rc = chdir(getenv("HOME"));
	#endif

	ifstream infile2;

	
	#if     _MSC_VER
	infile2.open(ini);   //MSVC
	#else
	infile2.open(ini);    		  //gcc
	#endif


	if(!infile2)
		{ofstream outfile2(ini);
		cout<<"\nIs GnuPlot installed? (1: Yes,  other: no)   ";
		vplot0=ReadKey();
		
		if(vplot !=0)
			{cout<<"\ngnuplot (version 4.0) needs to be installed to be able to plot the ASCII files.\n";
			cout<<"gnuplot (freeware) can be downloaded at http://www.gnuplot.info \n\n";}
			
		outfile2<<outtype0<<"\n"<<infofile0<<"\n"<<vplot0<<"\n";
		outtype=outtype0;
		infofile=infofile0;
		vplot=vplot0;}

	infile2>>outtype>>infofile>>vplot;
	infile2.close();
	
	if(outtype==0 || infofile==0 || vplot==0)
		{ofstream outfile2(ini); 
		outfile2<<outtype0<<"\n"<<infofile0<<"\n"<<vplot0<<"\n";
		outtype=outtype0;
		infofile=infofile0;
		vplot=vplot0;
		outfile2.close();}	
	
	#if     _MSC_VER
		#if MSini ==1
		_chdir(mainpath);			
		#endif
	#else
		rc = chdir(mainpath);
	#endif		
	
	typeAES=1;
	strcpy(difextension, ascextensionB);	
	
	int type=0;
	if(argc<2)
	{

	//**************User interface******************************************************
	cout<< "_______________________________________________________________________________";
	cout<< "\nConvert from PHI - Perkin Elmer Auger Electron Spectroscopy files into ASCII \n";
	cout << "\nPress:\n0) Exit\t\t\t\t3) Preferences";
	cout<<"\n1) Single conversion \t\t4) About\n2) Batch conversion\n";
	//**********************************************************************************

	type=ReadKey();
	}


if(argc>=2)
	{	for(int i=1; i<argc; i++)
			{DnD(argv[i]);}

	return 0; }

	if((type!=1) & (type!=2) & (type!=3) & (type!=4) )
		{cout<<"Bye Bye\n";
		return 0;}

	if(type==4)
		{cout<<"\n\n\nPhiConv converts 'PHI Surface Analysis Software' proprietary ASCII files \ninto conventional ASCII file\n\n"; 
		cout<<"Version: "<<version<<"\nSuggestions and bugs: Nicola Ferralis - feranick@gmail.com\n\n";
		cout<<"gnuplot (version 4.0) needs to be installed to be able to plot the ASCII files.\n";
		cout<<"gnuplot (freeware) can be downloaded at http://www.gnuplot.info \n\n";
	
		return main(0,0);}

	if (type==3)
		{

		int *tmp, *preftype;
		tmp=(int *)malloc(sizeof(int));
		preftype=(int *)malloc(sizeof(int));
		*preftype=1;
		
		while(*preftype!=0)
			{PreferencePanel();
			cout<<"  Type: (1 to 3: change individual settings)\n\t(9: restore default)   (0: exit)  ";
			*preftype=ReadKey();

			if (*preftype==1)
				{cout<<"\n\nWith this option you can specify the name of the output file:";
				cout<<"\n 1) Automatic: output is saved in a file with extension \".dat\".;";
				cout<<"\n 2) Manual: the program asks for the name of the output file.";		
				cout<<"\n\n (1: Automatic)  (2: Manual)  (other: no change) ";
				*tmp=outtype;
				outtype=ReadKey();
				if (outtype !=1 && outtype !=2)
					{cout<<" Value not updated\n";
					outtype=*tmp;}
				}
			
			if (*preftype==2)
				{cout<<"\nWith this option you can save the acquisition information in a separate file:";
				cout<<"\n 1) Saved: info saved in a file with extension \".info.txt\".;";		
				cout<<"\n 2) Not saved: the program does not save the info.";
				cout<<"\n\n (1: Saved)  (2: Not saved)  (other: no change) ";
				*tmp=infofile;
				infofile=ReadKey();
				if (infofile !=1 && infofile !=2)
					{cout<<" Value not changed\n";
					infofile=*tmp;}
				}		
		
			if (*preftype==3)
				{cout<<"\nDo you want the spectra to be plotted? ";
				cout<<"\n (GnuPlot needs to be installed to take advantage of this feature)";
				
				cout<<"\n (1: Plotted)  (2: NOT plotted)  (other: no change) ";
				*tmp=vplot;
				vplot=ReadKey();
				if(vplot==2)
					{cout<<" Some features (fitting) may not be available if plotting is not enabled. \n";}
				if(vplot!=1 && vplot!=2)
					{cout<<" Value not changed\n\n";
					vplot=*tmp;}
				}

			
			if (*preftype==9)
				{outtype=outtype0;
				infofile=infofile0;
				vplot=vplot0;
				cout<<"\n Default parameters succesfully restored!\n\n";}
			}
		free(tmp);	
		free(preftype);	

		#if     _MSC_VER
			#if MSini ==1
			_chdir(inipath);			
			#endif
		#else
		rc = chdir(getenv("HOME"));
		#endif
		
		ofstream outfile2(ini);
		outfile2<<outtype<<"\n"<<infofile<<"\n"<<vplot<<"\n";
		
		#if     _MSC_VER
			#if MSini ==1
			_chdir(mainpath);			
			#endif
		#else
		rc = chdir(mainpath);
		#endif
		
		outfile2.close();
		main(0,0);	
		}


//Converter

	if (type==1)
		{btype=1;
		single();}



//********* Batch **************************


	if (type==2)
		{btype=1;
		batch();}

return 0;}

//*********************************************************************
// Drag and drop conversion

int DnD(char *name)
	{
	
#if     _MSC_VER
	ifstream infile(name);		
	#else
	ifstream infile(name);		
	#endif
	if(!infile)
		{cout<<"\n file \""<< name<<"\" not found\n";
		return 0;}

	char *test, *tmp, *tmp2;
	
	test=(char *)malloc(sizeof(char[CMAXI]));
	tmp=(char *)malloc(sizeof(char[CMAXI]));
	tmp2=(char *)malloc(sizeof(char[CMAXI]));
	infile>>test;	

	infile.close();
	sprintf(tmp, "%s", name);
	tmp[strlen(tmp) - 4] = '\0';
	sprintf(tmp2, "%s%s", tmp, infoextension);

	if(strcmp(test,"FileName")==0)
	{	extractW32(name,tmp, tmp2);}

	if(strcmp(test,"TABLE")==0)
		{extract(name,tmp, tmp2);}
	
	free(test);	 
	free(tmp);
	free(tmp2);
	return 0;

	}

//------Routine to convert from DOS version of PHI software (.dif extension)---

acq extract(char* name, char* nameout, char *nameinfo)
	{
	#if     _MSC_VER
	ifstream infile(name);		//MSVC
	#else
	ifstream infile(name);		//gcc
	#endif

	static char test[20];	

		if(!infile)
		{if(btype==1)
			cout<<"\n file '"<< name<<"' not found\n ";		
		flag=0;
		return a;}
	
	char *nameOUT;
	nameOUT=(char *)malloc(sizeof(char[CMAXI]));	
	sprintf(nameOUT, "%s%s", nameout, datextension);	
	ofstream outfile(nameOUT);

	for (int w=0; w<156;w++)
		{infile>>test;}
	
	infile>>test>>a.acqtype;

	for (int f=0; f<49; f++)
		{infile>>test;}

	infile>>test>>a.sweepStart;

	infile>>test>>test;
	infile>>a.range>>test;
	a.sweepEnd= a.sweepStart+a.range;
	infile>>test;
	infile>>a.evPerStep;
	infile>>test>>test;
	infile>>a.timePerStep;

	steps=(int)(a.range/a.evPerStep);

	for (int i=0; i<37; i++)
		{infile>>test;}

#if     _MSC_VER
	double energy[Max];
	char countArray[Max][20];		//MSVC
#else
	double energy[steps];
	char countArray[steps][20];		//gcc
#endif
	
	char Arraytest[]={'0',',','\0'};
	char Arraytest2[]={'-','1',',','0','\0'};

	//Data Reading and analysis
	infile>>countArray[0];
	Comp=strcmp(countArray[0], Arraytest);
	
	if (Comp==0)
		{infile>>countArray[0];}
	if(Comp!=0)
		{strcpy(countArray[0],"  ");	}


	for(j=1; j<steps+1; j++)
		{infile>>test;
		infile>>countArray[j];
		Comp=strcmp( countArray[j], Arraytest);
		Comp2=strcmp( countArray[j], Arraytest2);
		if (Comp==0)
			{infile>>countArray[j];}
		if(Comp!=0)
			{strcpy(countArray[j],"  ");}
		if (Comp2==0)
			{steps=j-1;}
		}
	infile.close();

	for(j=0; j<steps+1; j++)
		{energy[j]= a.sweepStart+j*a.evPerStep;}

	for(j=0; j<steps+1; j++)
		{outfile<<energy[j]<<"\t"<<countArray[j]<<"\n";}
		
	outfile.close();
	sprintf(nameout, "%s", nameOUT);
	free(nameOUT);

	cout<<"\nName original file: "<<name;
	cout<<"\nData type= "<< a.acqtype;
	cout<<"\nSweep Start= "<< a.sweepStart<< " eV\nSweep End= "<<a.sweepEnd<<" eV";
	cout<<"\nNumber of sweeps= "<<a.noOfSweep;
	cout<<"\nRange= "<<a.range<<" eV\neV per Step= "<<a.evPerStep<<"\nTime per Step= "<<a.timePerStep<<" s";
	cout<<"\ncurve saved in: "<<nameout<<"\n";

	if(infofile==1)
		{
		
		ofstream outinfo(nameinfo);
		cout<<"Information saved in: "<<nameinfo<<"\n\n";
		outinfo<<"----------------------------------------------------------\n";
		outinfo<<"Name file: "<<name<<"\n";
		
		outinfo<<"\nData type= "<< a.acqtype;
		outinfo<<"\nSweep Start= "<< a.sweepStart<< " eV\nSweep End= "<<a.sweepEnd<<" eV";
		outinfo<<"\nRange= "<<a.range<<" eV\neV per Step= "<<a.evPerStep<<"\nTime per Step= "<< a.timePerStep<<" s";
		outinfo<<"\n\nCurve saved in: "<<name<<"\n";
		outinfo<<"-----------------------------------------------------------\n";
		outinfo.close();
					}
	
	return a;
}


//------Routine to convert from Windows version of PHI software (.asc extension)---
acq extractW32(char* name, char* nameout, char *nameinfo)
	{
		
	#if     _MSC_VER
	ifstream infile(name);		//MSVC
	#else
	ifstream infile(name);		//gcc
	#endif
		
	
	if(!infile)
		{cout<<"\n file '"<< name<<"' not found\n ";
		flag=0;
		return a;}

	char *nameOUT;
	nameOUT=(char *)malloc(sizeof(char[CMAXI]));	
	sprintf(nameOUT, "%s%s", nameout, datextension);
	ofstream outfile(nameOUT);	

	int j;
	static int steps;
	static char test[CMAXI];
	strcpy(a.comment, "");
	
	while(!infile.eof())
		{infile>>test;
		if (strcmp(test,"FileName")==0)
			{infile>>test>>a.nameorigfile;}	
		if (strcmp(test,"DataMode")==0)
			{infile>>test>>a.acqtype;}
		if (strcmp(test,"Comment")==0)
			{infile>>test;
			while(strcmp(test, "AcqMode")!=0)
				{infile>>test;	
				if(strcmp(test, "AcqMode")!=0)
					{strcat(a.comment, test);
					strcat(a.comment, " ");}
							
				}
			
			}	
		if (strcmp(test,"RegTime")==0)
			{break;}
		}
	
	for (int w=0; w<14;w++)
		{infile>>test;}

	infile>>(a).noOfSweep>>(a).evPerStep;
	infile>>test>>(a).sweepStart>>(a).sweepEnd;
	(a).range= (a).sweepEnd-(a).sweepStart;
	steps=(int)((a).range/(a).evPerStep);
	infile>>test>>test>>(a).timePerStep;

	for(int w2=0; w2<3; w2++)
		{infile>>test;}

	#if     _MSC_VER
	double energy[Max];
	double Array[Max];		//MSVC
	
	#else
	
	double energy[steps];
	double Array[steps];		//gcc
	
	#endif

	for(j=0; j<steps+1; j++)
		{infile>>energy[j];}
	
	infile>>test>>test;

	for(j=0; j<steps+1; j++)
		{infile>>Array[j];}
	
	infile.close();

	for(j=0; j<steps+1; j++)
		{outfile<<energy[j]<<"\t"<<Array[j]<<"\n";}
	
	outfile.close();
	//snprintf(nameout, CMAXI, "%s", nameOUT);
	strcpy(nameout,nameOUT);
	free(nameOUT);

	cout<<"\nName original file: "<<name;
	cout<<"\nComment: "<<a.comment;
	cout<<"\nData type= "<< a.acqtype;
	cout<<"\nSweep Start= "<< a.sweepStart<< " eV\nSweep End= "<<a.sweepEnd<<" eV";
	cout<<"\nNumber of sweeps= "<<a.noOfSweep;
	cout<<"\nRange= "<<a.range<<" eV\neV per Step= "<<a.evPerStep<<"\nTime per Step= "<<a.timePerStep<<" s";
	cout<<"\nCurve saved in: "<<nameout<<"\n";
	cout<<"\nInformation saved in: "<<nameinfo<<"\n\n";

	if(infofile==1)
		{
		
		ofstream outinfo(nameinfo, ios::app);
		outinfo<<"----------------------------------------------------------\n";
	
		outinfo<<"\nName original file: "<<name;
		outinfo<<"\nComment: "<<a.comment;
		outinfo<<"\nData type= "<< a.acqtype;
		outinfo<<"\nSweep Start= "<< a.sweepStart<< " eV\nSweep End= "<<a.sweepEnd<<" eV";
						
		outinfo<<"\nNumber of sweeps= "<<a.noOfSweep;
		outinfo<<"\nRange= "<<a.range<<" eV\neV per Step= "<<a.evPerStep<<"\nTime per Step= "<< a.timePerStep<<" s";
		outinfo<<"\n\nCurve saved in: "<<nameout<<"\n";
		outinfo<<"-----------------------------------------------------------\n";
		outinfo.close();
		
					}
	return a;
}
//////////////////////////////////////////////////////////////////////////////////
int single()
	{	
		char *nameIN, *nameout, *name, *nameinfo;
		nameIN=(char *)malloc(sizeof(char[CMAXI]));	
		nameout=(char *)malloc(sizeof(char[CMAXI]));	
		name=(char *)malloc(sizeof(char[CMAXI]));	
		nameinfo=(char *)malloc(sizeof(char[CMAXI]));

		flag=1;
		cout<<"name file (WITHOUT the extension): ";
		cin>> nameIN;

		if (outtype==1)
			{sprintf(nameout, "%s", nameIN);}
		else
			{cout<<"name file output (with extension): ";
			cin>>nameout;
			}

		formatRec(nameIN, name);
		sprintf(nameinfo, "%s%s", nameIN, infoextension);
	
		if(typeAES==0)
			{
			extract(name, nameout, nameinfo);}
		else
			{
			extractW32(name, nameout, nameinfo);}

		
			if(flag!=0)
				{if(vplot==1)		
					{SleepP();
					Plot(nameout);}
				closePlot();
				}
		free(nameIN);
		free(nameout);
		free(name);
		free(nameinfo);
	return main(0,0);}


int batch()
	{	//static char nameIN[CMAXI], nameout[CMAXI], name[CMAXI], nametmp[CMAXI];
		int f1;
		flag=1;
		char *nameIN, *nameout, *name, *nametmp, *nameinfo;
		nameIN=(char *)malloc(sizeof(char[CMAXI]));	
		nameout=(char *)malloc(sizeof(char[CMAXI]));	
		name=(char *)malloc(sizeof(char[CMAXI]));	
		nametmp=(char *)malloc(sizeof(char[CMAXI]));
		nameinfo=(char *)malloc(sizeof(char[CMAXI]));		

		cout<<"The file must have the name formatted as: \nbasename#"<<difextension<<"\nwhere # is the increasing, progressive number of the scan.\n";
		cout<<"Example of progressive files: data1"<<difextension<<", data2"<<difextension<<", ...\n";
		cout<<"\nFile base name: ";
		cin>> nameIN;
		cout<<"Number first file: ";
		f1=ReadKey();
		

		sprintf(nameinfo, "%s_%d_batch%s"  , nameIN, f1,infoextension);
		//for (f=f1; f<=f2; f++)
		while (flag!=0)
			{sprintf(nametmp, "%s%d", nameIN, f1); 
			sprintf(nameout,"%s%d"  , nameIN, f1); 			
			formatRec(nametmp, name);
			//cout<<name<<"\n";
			if(typeAES==0)
				{extract(name, nameout, nameinfo);}
			else
				{extractW32(name, nameout, nameinfo);}
			btype=2;
			//if(flag==0)
			//	{break;}	
			f1++;		
			}
		
		free(nameIN);
		free(nameout);
		free(name);
		free(nametmp);
		free(nameinfo);

		return main(0,0);
		}

//////////////////////////////////////////////////////////////////////////////////

void Plot(char* nameout)
{	
	
	#if     _MSC_VER
    command = popen("pgnuplot","w");
	#else
	command = popen("gnuplot","w");
	#endif

 	fprintf(command,"set mouse\n");
	fprintf(command,"set autoscale\n");     
	fprintf(command,"set grid\n");
	fprintf(command,"set title 'Use the right button of your mouse to zoo. Type p to unzoom'\n");
	fprintf(command,"plot \"%s\" with lines\n",nameout); 
    	fflush(command);
}


void closePlot()

{	if(vplot==1)
		{int *tmp;
		tmp=(int *)malloc(sizeof(int));
		
		cout<<"(1: for chemical analysis;  else: close) ";
		*tmp=ReadKey();
		if(*tmp==1)
			ChemAnal();
		else
			{}	
		fprintf(command,"q\n"); 
        	fflush(command);
		free(tmp);}
	else
		{}
}

//************************************
// Keyboard input I/O

int ReadKey()
{	char tkc[10];
	int tk;
	cin>>tkc;
		
	tk=(int) atof(tkc);
	if(tk<0)
		{flag=0;
		return 10;}
	else
		{}

	return tk;
}

float ReadKeyF()
{	char tkc[10];
	float tk;
	cin>>tkc;
		
	tk= (float) atof(tkc);
	if(tk<0)
		{return 10;}
	else
		{}

	return tk;
}


void SleepP()
{	
	#if     _MSC_VER
		Sleep(200);
	#else
		usleep(200000); 
	#endif
}

//***********************************


void formatRec(char *nameIN, char *name)	
	{

	sprintf(name, "%s%s", nameIN, ascextensionB);	
	ifstream infile;
	
	#if     _MSC_VER
	infile.open(name);		//MSVC
	#else
	infile.open(name);		//gcc
	#endif

	
	if(!infile)
		{sprintf(name, "%s%s", nameIN, ascextensionS);
		typeAES=1;		
		}
	infile.close();

	#if     _MSC_VER
	infile.open(name);		//MSVC
	#else
	infile.open(name);		//gcc
	#endif

	if(!infile)
		{sprintf(name, "%s%s", nameIN, difextensionB);
		typeAES=0;
		}
	infile.close();

	#if     _MSC_VER
	infile.open(name);		//MSVC
	#else
	infile.open(name);		//gcc
	#endif

	if(!infile)
		{sprintf(name, "%s%s", nameIN, difextensionS);
		typeAES=0;
		}
	infile.close();
	}

//***********************************

void PreferencePanel()

{	cout<<"\n****************************************************************************\n \"Preferences\" \n";
	//cout<<"\n1) Setting for the name of the output file: ";
	cout<<"\n1)";
	if(outtype ==1)
		{cout <<" Output is saved in a file with extension \".dat\".\n";}		
	if (outtype ==2)
		{cout<<" The program asks for the name of the output file.\n";}
	
	//cout<<"2) Info file status: ";
	cout<<"\n2)";
	if(infofile ==1)
		{cout <<" Info saved in a file with extension \".info.txt\". \n";}
	if(infofile ==2)
		{cout<<" Info file not saved.\n";}	
	
	
	cout<<"\n3) Spectra are currently: ";
	if(vplot ==1)
		{cout <<" plotted. \n";}
	if (vplot ==2)
		{cout<<" NOT plotted.\n\n";}	
	cout<<"\n*****************************************************************************\n";
}


////////////////////////////////////////////////

int ChemAnal()
{
	if(vplot==1)
		{mouse a;
	
		
		a.x=1.0;
		
		while(a.x>0.0)
			{a=det(1);
			cout<<a.x<<"\n";
			ChemID(a.x);}
		}
	else 
		{cout<<"\nPlease enable plotting (in the settings menu) to change range. \n";}
	return 0;
}

void ChemID(double a)
	{
	elcond(a, 92, "Si");
	elcond(a, 76, "SiO2");	
	elcond(a, 503, "Oxygen");	
	elcond(a, 271, "Carbon");
	elcond(a, 239, "Au");
	elcond(a, 61, "Ni");
	elcond(a, 64, "Pt");
	elcond(a, 179, "B");
	elcond(a, 379, "N");
	elcond(a, 152, "S");
	elcond(a, 181, "Cl");
	elcond(a, 215, "Ar");
	elcond(a, 418, "Ti");
	elcond(a, 186, "Mo");	
	elcond(a, 179, "Ta");
	elcond(a, 120, "P");
	elcond(a, 239, "Au");	
	elcond(a, 179, "W");	
	}

void elcond(double a, double en, const char *el)
	{double p;
	if(en>a)
		{p=6.0*en/a;}
	else	
		{p=6.0*a/en;}

	if(a<en+p && a>en-p)
		{cout<<"Element: "<<el<<"\t(real: "<<en<<" eV)\n";}	
	else
		{}
	}

mouse det(int b)
	{int dm =0;	
	mouse a;
	a.x=0.0;	
		
		
		#if     _MSC_VER
		#else
			if (mkfifo(GPFIFO, 0700)) 
				{
    				if (errno != EEXIST) 
					{perror(GPFIFO);
					unlink(GPFIFO);
					return a;}
				}
		#endif		
		

		

		#if     _MSC_VER			
			cout<< "\n Enter the X coordinate (or 0 to exit): ";
	 		a.x=ReadKeyF();		    		
		#else
			
			fprintf(command, "set print \"%s\"\n", GPFIFO);
			fflush(command);
	
			if (NULL == (gpin = fopen(GPFIFO,"r"))) 
				{perror(GPFIFO);
	   			pclose(command);
	   			return a;}
		
			

			fprintf(command,"set mouse\n");
			fprintf(command,"replot\n");
			fflush(command);			
			cout<<"\n";
			fprintf(command, "pause mouse ' Click mouse in the X coordinate (or a value outside the scale to exit).  '\n");
			
	   		fflush(command);
	       		fprintf(command, "print MOUSE_X\n");
	    		fflush(command);
			dm=fscanf(gpin, "%lf", &a.x);
			cout<<"\n\nEnergy (eV): ";
			
			fclose(gpin);
			unlink (GPFIFO);
			
		#endif
			
		
		
	return a;
}
