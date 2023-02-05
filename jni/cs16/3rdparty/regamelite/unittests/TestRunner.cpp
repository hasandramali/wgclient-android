// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "precompiled.h"
#include "cppunitlite/GradleAdapter.h"

int main(int argc, char* argv[]) {
	printf("TestRunner: main()\n");

	GradleAdapter a;
	int res = a.testsEntryPoint(argc, argv);

#ifdef _BUILD_FROM_IDE
	system("PAUSE");
#endif

	return res;
}
