#include "newsagency.h"
#include <fstream>
#include <vector>
#include "buffile.h"
#include "delim.h"
#include "iobuffer.h"
#include "recfile.h"
#include "varlen.h"

int shownewsagency() {
	ifstream ifs("listOfNewsAgency.txt");
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
			NewsAgency na;
			ifs >> na;
			cout << na;
		}
		return 0;
	}
	for (int i = 0; i < 10; i++) {
		NewsAgency na;
		ifs >> na;
		cout << na;
	}
	return 0;
}