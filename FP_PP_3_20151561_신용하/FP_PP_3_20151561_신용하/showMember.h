#include "member.h"
#include <fstream>
#include <vector>
#include "buffile.h"
#include "delim.h"
#include "iobuffer.h"
#include "recfile.h"
#include "varlen.h"

int showmember() {
	ifstream ifs("listOfMember.txt");
	if (ifs.fail()) {
		cout << "File Open error!" << endl;
		return -1;
	}

	int n;
	ifs >> n;
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	DelimFieldBuffer buffer('|', STDMAXBUF);
	if (n < 10) {
		for (int i = 0; i < n; i++) {
			Member m;
			ifs >> m;
			cout << m;
		}
		return 0;
	}
	for (int i = 0; i < 10; i++) {
		Member m;
		ifs >> m;
		cout << m;
	}
	return 0;
}