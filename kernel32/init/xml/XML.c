//
int InitSystemPass2_XML(void)
{
	//
	initSystemLogging();
	fdc_init();
	rd_init();
	hd_init();
	partition_table_support_init();
	hwdet();
	StartShellProcess();
}

