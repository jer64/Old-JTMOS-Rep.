// XML parser for the JTMOS
#include <stdio.h>

//
int getStringInsideDefinition(const char *str, char *funcName,
	char definitionChar)
{
	int i,i2,l;

	// Get string inside <*...*>.
	l = strlen(str);
	for(i=1,i2=0; str[i]!=definitionChar && i<l; i++,i2++) {
		funcName[i2] = str[i];
	}
	// Add end of string.
	funcName[i2] = 0;
	return 0;
}

//
int parse_file(const char *fname, int verbose)
{
	FILE *f;
	char str[8192],tmp[8192],str2[8192],funcName[8192];
	int l,element_nr,l_element_nr;

	//
	if(fname==NULL) {
		return 1;
	}

	//
	f = fopen(fname, "rb");
	if(f==NULL) {
		return 2;
	}
	for(element_nr=0,l_element_nr=0; !feof(f); ) {
		fgets(&tmp, 8192, f);
		strcpy(str, "");
		strcpy(str2, "");
		sscanf(tmp, "%s %s", &str, &str2);
		if(verbose>=1) {
			fprintf(stderr, "str=%s, str2=%s\n",
				str, str2);
		}

		// <XML_ITEM>
		// </XML_ITEM>

		// Begin item.
		if( str[0]=='<' && isalnum(str[1]) ) {
			l_element_nr = element_nr;
			element_nr++;
			if(verbose>=1)
				fprintf(stderr, "element_nr=%d, l_element_nr=%d\n",
					element_nr,l_element_nr);
			// Module name.
			if(element_nr==1 && l_element_nr==0) {
			}
			// Function definition begins.
			if(element_nr==2 && l_element_nr==1) {
				//
				getStringInsideDefinition(str, funcName, '>');
				// Produce/open C-function (output):
				fprintf(stdout, "//\nint %s(void)\n{\n\t//\n",
					funcName);
			}
			// At element stage #3 can accept element calls.
			if(element_nr==3) { // function call
				getStringInsideDefinition(str, funcName, '>');
				// Produce C-function call (output):
				fprintf(stdout, "\t%s();\n",
					funcName);
			}
		}
		// End item.
		if(str[0]=='<' && str[1]=='/') {
			l_element_nr = element_nr;
			element_nr--;
			// Close function definition.
			if(element_nr==1 && l_element_nr==2) {
				//
				getStringInsideDefinition(str+2, funcName, '>');
				// Close C-function (output):
				fprintf(stdout, "}\n\n",
					funcName);
			}
		}
	}
	fclose(f);

	//
	return 0;
}

//
int main(int argc, char **argv)
{
	int verbose,i;

	//
	if(argc<2) {
		fprintf(stderr, "xmlparse.c [-v] [xml-file] > output_c_file.c\n");
		fprintf(stderr, "Options:\n");
		fprintf(stderr, " [-v] [-vv] [-vvv]	Choose verbose-level.\n");
		fprintf(stderr, "");
	}

	// Check for options.
	for(verbose=0,i=1; i<argc; i++) {
		if(argv[i][0]!='-') { break; }
		if(argv[i], "-v") {
			verbose = 1;
		}
		if(argv[i], "-vv") {
			verbose = 2;
		}
		if(argv[i], "-vvv") {
			verbose = 3;
		}
	}
	// Pass first argument without option indentifier plus verbose level.
	parse_file(argv[i],verbose);

	//
}
