
// MICDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MIC.h"
#include "MICDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <vector>
#include <sstream>




#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMICDlg dialog



CMICDlg::CMICDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MIC_DIALOG, pParent)
	, Path(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	 outBufAudio = NULL;
	 title = "";
}

void CMICDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, Path);
}


BEGIN_MESSAGE_MAP(CMICDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTMIC_OUTOPEN, &CMICDlg::OnBnClickedButmicOutopen)
	ON_BN_CLICKED(IDC_BUTMIC_OUTCLOSE, &CMICDlg::OnBnClickedButmicOutclose)
	ON_BN_CLICKED(IDC_BUTMIC_OUTPLAY, &CMICDlg::OnBnClickedButmicOutplay)
	ON_MESSAGE(WOM_DONE, &CMICDlg::OnOutDone)
	ON_BN_CLICKED(IDC_BUTMIC_OUTSTOP, &CMICDlg::OnBnClickedButmicOutstop)
	ON_EN_CHANGE(IDC_EDIT1, &CMICDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CMICDlg message handlers

BOOL CMICDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMICDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMICDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // device context for painting
		CString cs(title.c_str());
		// tu rysujemy
		if (title != "")
			dc.TextOutW(10, 10,cs);
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMICDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMICDlg::OnBnClickedButmicOutopen()
{

	if (title!="")
	{
		MessageBox(L"          Najpierw zamknij stary plik");
		return;
	}

	if(BUFFER_SIZE.size()>0)
	{
		aktualny_element = 0;
		BUFFER_SIZE.clear();
		freq.clear();
	}

	unsigned long result;
	WAVEFORMATEX waveFormat;
	// --- Inicjalizacja wyjscia dla Audio ------------------
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nChannels = 1;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 8;
	waveFormat.nBlockAlign = waveFormat.nChannels * (waveFormat.wBitsPerSample / 8);
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;
	// --- Otworzenie urzadzenia audio wyjscie --------------
	result = waveOutOpen(&outHande, WAVE_MAPPER, &waveFormat, (DWORD)(this->m_hWnd), 0, CALLBACK_WINDOW);
	if (result) 
		MessageBox(L"Blad otwierania wyjsciowego urzadzenia audio!!!");
	
	
	
	
	// podanie adresu i 
	//MessageBox(L"Podaj Ÿród³o dzwiêku");

	// pobieranie z pliku 

	CT2CA x(Path);
	std::string location(x);


	INT_PTR InputBox(LPCTSTR szTitle,
		LPCTSTR szPrompt,
		LPTSTR szResult,
		DWORD nResultSize,
		bool bMultiLine = false,
		HWND hwndParent = 0);



	std::ifstream inputFile(location);
	if (!inputFile.is_open())
	{
		//location = location + ".txt";
		//std::ifstream inputFile2(location);
		//if (!inputFile.is_open())
		//{
			MessageBox(L"          Brak Pliku");
			waveOutClose(outHande);
			return;
		//}
	}

	std::string line;
	getline(inputFile, title);
	getline(inputFile, line);
	tempo = std::stoi(line);
	getline(inputFile, line);
	oktawa = std::stoi(line);

	while (!inputFile.eof())
	{
		int count = 0;
		getline(inputFile, line); // pobieram linie
		while (line[count] != NULL)
		{
			int buffers = line[count] - '0';
			count++;
			while (line[count] > 47 && line[count] < 58) // dopoki jest cyfra (np 16 32  128)
			{
				buffers *= 10;
				buffers+= line[count] - '0';
				count++;
			}
			if (line[count]=='.')
			{
				BUFFER_SIZE.push_back((10560000 / (tempo*buffers))*1.5);
				count++;
			}
			else
				BUFFER_SIZE.push_back(10560000 / (tempo*buffers));

			float fq;
			switch (line[count])
			{
			case 'C':
				count++;
				if (line[count] == '#')
				{
					fq = 138.6*pow(2, oktawa);
					count++;
				}
				else
					if (line[count] == 'b')
					{
						fq = 123.5*pow(2, oktawa);
						count++;
					}
					else
						fq = 130.8*pow(2, oktawa);
				break;

			case 'D':
				count++;
				if (line[count] == '#')
				{
					fq = 155.55*pow(2, oktawa);
					count++;
				}
				else
					if (line[count] == 'b')
					{
						fq = 138.6*pow(2, oktawa);
						count++;
					}
					else
						fq = 146.85*pow(2, oktawa);
				break;

			case 'E':
				count++;
				if (line[count] == '#')
				{
					fq = 174.6*pow(2, oktawa);
					count++;
				}
				else
					if (line[count] == 'b')
					{
						fq = 155.55*pow(2, oktawa);
						count++;
					}
					else
						fq = 164.8*pow(2, oktawa);
				break;

			case 'F':
				count++;
				if (line[count] == '#')
				{
					fq = 185.0*pow(2, oktawa);
					count++;
				}
				else
					if (line[count] == 'b')
					{
						fq = 164.8*pow(2, oktawa);
						count++;
					}
					else
						fq = 174.6*pow(2, oktawa);
				break;

			case 'G':
				count++;
				if (line[count] == '#')
				{
					fq = 207.65*pow(2, oktawa);
					count++;
				}
				else
					if (line[count] == 'b')
					{
						fq = 185.0*pow(2, oktawa);
						count++;
					}
					else
						fq = 196.0*pow(2, oktawa);
				break;

			case 'A':
				count++;
				if (line[count] == '#')
				{
					fq = 233.1*pow(2, oktawa);
					count++;
				}
				else
					if (line[count] == 'b')
					{
						fq = 207.65*pow(2, oktawa);
						count++;
					}
					else
						fq = 220.0*pow(2, oktawa);

				break;

			case 'H':
				count++;
				if (line[count] == '#')
				{
					fq = 261.6*pow(2, oktawa);
					count++;
				}
				else
					if (line[count] == 'b')
					{
						fq = 233.1*pow(2, oktawa);
						count++;
					}
					else
						fq = 246.95*pow(2, oktawa);
				break;

			case 'B':
				count++;
				if (line[count] == '#')
				{
					fq = 261.6*pow(2, oktawa);
					count++;
				}
				else
					if (line[count] == 'b')
					{
						fq = 233.1*pow(2, oktawa);
						count++;
					}
					else
						fq = 246.95*pow(2, oktawa);
				break;

			default:
				fq = 1;
				count++;
				break;
			}

			if (line[count] != ' ')
			{
				fq *= pow(2, line[count] - '0');
				count++;
			}

			freq.push_back(fq);
			count++;
		}
	}

	int czas_w_sekundach=0;
	int czas_w_minutach=0;
	// Obliczenie dlugosci
	for (int i = 0; i < BUFFER_SIZE.size(); i++)
		czas_w_sekundach += BUFFER_SIZE[i];
	czas_w_sekundach /= 44000;
	czas_w_minutach = czas_w_sekundach / 60;
	czas_w_sekundach = czas_w_sekundach % 60;
	czas_w_sekundach++;

// wyswietl tytul
	std::stringstream info;
	info << "Tytu³: " << title << "\n" << "Czas trwania: " << czas_w_minutach << "m " << czas_w_sekundach << "s";

	MessageBoxA(NULL, info.str().c_str(), "Info", MB_OK);
	aktualny_element = 0;
	unsigned char a;
	outBufAudio = &a;
}



void CMICDlg::OnBnClickedButmicOutclose()
{
	std::stringstream info;
	if (outBufAudio!=NULL){
		freq.clear();
		title = "";
		BUFFER_SIZE.clear();
		info << "          Zamknieto " << title;
		MessageBoxA(NULL, info.str().c_str(), "Zamykanie", MB_OK);
		aktualny_element = 0;
		outBufAudio = NULL;
	}
	else
	{
		info << "        Nie zostal otwarty zaden plik!";
		MessageBoxA(NULL, info.str().c_str(), "Zamykanie", MB_OK);
	}
	waveOutClose(outHande);
}

float dzwiek = 1000;

void CMICDlg::OnBnClickedButmicOutplay()
{
	if (aktualny_element == 0)
		zagrajmi();
	
}

void CMICDlg::zagrajmi()
{
	if (aktualny_element != 0)
		if (aktualny_element >= freq.size() || aktualny_element == (-1)) // jesli przekroczono wektor elementow
		{
			std::stringstream info;
			info << "        Koniec Utworu " << title;
			if (aktualny_element != (-1))
				MessageBoxA(NULL, info.str().c_str(), "Uwaga!", MB_OK);
			aktualny_element = 0;
			return;
		}


	if (BUFFER_SIZE.size() > 0)
	{

		outBufAudio = new unsigned char[BUFFER_SIZE[aktualny_element]];

		const double pi = 3.1415926535;
		double sample_rate = 44100.0;

		for (int i = 0; i < BUFFER_SIZE[aktualny_element]; i++)
			outBufAudio[i] = ((unsigned char) 170.0*sin((2.0*pi * freq[aktualny_element])*(i / sample_rate)));

		outBuffer.lpData = (LPSTR)(this->outBufAudio);
		outBuffer.dwBufferLength = (BUFFER_SIZE[aktualny_element]);
		outBuffer.dwFlags = 0;
	}

	int err;
	if (err = waveOutPrepareHeader(outHande, &outBuffer, sizeof(WAVEHDR)))
	{
		MessageBox(L"        Najpierw wybierz utwor!");
		return;
	}

	waveOutWrite(outHande, &outBuffer, sizeof(WAVEHDR));
	aktualny_element++;

}

LRESULT CMICDlg::OnOutDone(WPARAM wParam, LPARAM lparam)
{
	//MessageBox(L"Koniec grania nuty");
	delete outBufAudio;
	zagrajmi();
	return(0);
}

void CMICDlg::OnBnClickedButmicOutstop()
{
	if(aktualny_element!=0){
		std::stringstream info;
		info << "        Zatrzymano Utwor " << title;
		MessageBoxA(NULL, info.str().c_str(), "Uwaga!", MB_OK);
		aktualny_element = -1;
		return;
	}
	else
	{
		std::stringstream info;
		info << "          NIC NIE ODTWARZAM ";
		MessageBoxA(NULL, info.str().c_str(), "Uwaga!", MB_OK);
	}
}


void CMICDlg::OnEnChangeEdit1()
{
	this->UpdateData(true);
	
}
