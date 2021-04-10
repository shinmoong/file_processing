#include "member.h"
#include <fstream>
#include <vector>
#include "buffile.h"
#include "delim.h"
#include "iobuffer.h"
#include "recfile.h"
#include "varlen.h"

using namespace std;

int membertest() {
	ifstream ifs("listOfMember.txt");
	if (ifs.fail()) {
		cout << "File Open error!" << endl;
		return -1;
	}

	int n;
	ifs >> n;
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	DelimFieldBuffer buffer('|', STDMAXBUF);
	RecordFile <Member> MemberFile(buffer);
	char file[30] = "fileOfMember.dat";
	MemberFile.Create(file, ios::out | ios::trunc);
	for (int i = 0; i < n; i++) {
		Member s;
		ifs >> s;
		int recaddr;
		if ((recaddr = MemberFile.Write(s)) == -1) {
			cout << "Write Error!" << endl;
		}
	}
	MemberFile.Close();

	MemberFile.Open(file, ios::in);
	for (int i = 0; i < 10; i++) {
		Member m;
		if (MemberFile.Read(m) == -1) {
			break;
		}
		cout << m;
	}
	MemberFile.Close();

	return 0;
}