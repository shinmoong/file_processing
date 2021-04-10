#include "newsagency.h"
#include <fstream>
#include <vector>
#include "buffile.h"
#include "delim.h"
#include "iobuffer.h"
#include "recfile.h"
#include "varlen.h"

using namespace std;

int newsagencytest() {
	ifstream ifs("listOfNewsAgency.txt");
	if (ifs.fail()) {
		cout << "File Open error!" << endl;
		return -1;
	}

	int n;
	ifs >> n;
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	DelimFieldBuffer buffer('|', STDMAXBUF);
	RecordFile <NewsAgency> NewsAgencyFile(buffer);
	char file[30] = "fileOfNewsAgency.dat";
	NewsAgencyFile.Create(file, ios::out | ios::trunc);
	for (int i = 0; i < n; i++) {
		NewsAgency na;
		ifs >> na;
		int recaddr;
		if ((recaddr = NewsAgencyFile.Write(na)) == -1) {
			cout << "Write Error!" << endl;
		}
	}
	NewsAgencyFile.Close();

	NewsAgencyFile.Open(file, ios::in);
	for (int i = 0; i < 10; i++) {
		NewsAgency n;
		if (NewsAgencyFile.Read(n) == -1) {
			break;
		}
		cout << n;
	}
	NewsAgencyFile.Close();

	return 0;
}