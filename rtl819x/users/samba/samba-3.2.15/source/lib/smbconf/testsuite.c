/*
 *  Unix SMB/CIFS implementation.
 *  libsmbconf - Samba configuration library: testsuite
 *  Copyright (C) Michael Adam 2008
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "includes.h"

static void print_strings(const char *prefix,
			  uint32_t num_strings, const char **strings)
{
	uint32_t count;

	if (prefix == NULL) {
		prefix = "";
	}

	for (count = 0; count < num_strings; count++) {
		printf("%s%s\n", prefix, strings[count]);
	}
}

static bool test_get_includes(struct smbconf_ctx *ctx)
{
	WERROR werr;
	bool ret = false;
	uint32_t num_includes = 0;
	char **includes = NULL;
	TALLOC_CTX *mem_ctx = talloc_stackframe();

	printf("test: get_includes\n");
	werr = smbconf_get_global_includes(ctx, mem_ctx,
					   &num_includes, &includes);
	if (!W_ERROR_IS_OK(werr)) {
		printf("failure: get_includes - %s\n", dos_errstr(werr));
		goto done;
	}

	printf("got %u includes%s\n", num_includes,
	       (num_includes > 0) ? ":" : ".");
	print_strings("", num_includes, (const char **)includes);

	printf("success: get_includes\n");
	ret = true;

done:
	TALLOC_FREE(mem_ctx);
	return ret;
}

static bool test_set_get_includes(struct smbconf_ctx *ctx)
{
	WERROR werr;
	uint32_t count;
	bool ret = false;
	const char *set_includes[] = {
		"/path/to/include1",
		"/path/to/include2"
	};
	uint32_t set_num_includes = 2;
	char **get_includes = NULL;
	uint32_t get_num_includes = 0;
	TALLOC_CTX *mem_ctx = talloc_stackframe();

	printf("test: set_get_includes\n");

	werr = smbconf_set_global_includes(ctx, set_num_includes, set_includes);
	if (!W_ERROR_IS_OK(werr)) {
		printf("failure: get_set_includes (setting includes) - %s\n",
		       dos_errstr(werr));
		goto done;
	}

	werr = smbconf_get_global_includes(ctx, mem_ctx, &get_num_includes,
					   &get_includes);
	if (!W_ERROR_IS_OK(werr)) {
		printf("failure: get_set_includes (getting includes) - %s\n",
		       dos_errstr(werr));
		goto done;
	}

	if (get_num_includes != set_num_includes) {
		printf("failure: get_set_includes - set %d includes, got %d\n",
		       set_num_includes, get_num_includes);
		goto done;
	}

	for (count = 0; count < get_num_includes; count++) {
		if (!strequal(set_includes[count], get_includes[count])) {
			printf("expected: \n");
			print_strings("* ", set_num_includes, set_includes);
			printf("got: \n");
			print_strings("* ", get_num_includes,
				      (const char **)get_includes);
			printf("failure: get_set_includes - data mismatch:\n");
			goto done;
		}
	}

	printf("success: set_includes\n");
	ret = true;

done:
	TALLOC_FREE(mem_ctx);
	return ret;
}

static bool test_delete_includes(struct smbconf_ctx *ctx)
{
	WERROR werr;
	bool ret = false;
	const char *set_includes[] = {
		"/path/to/include",
	};
	uint32_t set_num_includes = 1;
	char **get_includes = NULL;
	uint32_t get_num_includes = 0;
	TALLOC_CTX *mem_ctx = talloc_stackframe();

	printf("test: delete_includes\n");

	werr = smbconf_set_global_includes(ctx, set_num_includes, set_includes);
	if (!W_ERROR_IS_OK(werr)) {
		printf("failure: delete_includes (setting includes) - %s\n",
		       dos_errstr(werr));
		goto done;
	}

	werr = smbconf_delete_global_includes(ctx);
	if (!W_ERROR_IS_OK(werr)) {
		printf("failure: delete_includes (deleting includes) - %s\n",
		       dos_errstr(werr));
		goto done;
	}

	werr = smbconf_get_global_includes(ctx, mem_ctx, &get_num_includes,
					   &get_includes);
	if (!W_ERROR_IS_OK(werr)) {
		printf("failure: delete_includes (getting includes) - %s\n",
		       dos_errstr(werr));
		goto done;
	}

	if (get_num_includes != 0) {
		printf("failure: delete_includes (not empty after delete)\n");
		goto done;
	}

	werr = smbconf_delete_global_includes(ctx);
	if (!W_ERROR_IS_OK(werr)) {
		printf("failuer: delete_includes (delete empty includes) - "
		       "%s\n", dos_errstr(werr));
		goto done;
	}

	printf("success: delete_includes\n");
	ret = true;

done:
	return ret;
}

static bool torture_smbconf_txt(void)
{
	WERROR werr;
	bool ret = true;
	struct smbconf_ctx *conf_ctx = NULL;
	TALLOC_CTX *mem_ctx = talloc_stackframe();

	printf("test: text backend\n");

	printf("test: init\n");
	werr = smbconf_init_txt(mem_ctx, &conf_ctx, NULL);
	if (!W_ERROR_IS_OK(werr)) {
		printf("failure: init failed: %s\n", dos_errstr(werr));
		ret = false;
		goto done;
	}
	printf("success: init\n");

	ret &= test_get_includes(conf_ctx);

	smbconf_shutdown(conf_ctx);

	printf("%s: text backend\n", ret ? "success" : "failure");

done:
	TALLOC_FREE(mem_ctx);
	return ret;
}

static bool torture_smbconf_reg(void)
{
	WERROR werr;
	bool ret = true;
	struct smbconf_ctx *conf_ctx = NULL;
	TALLOC_CTX *mem_ctx = talloc_stackframe();

	printf("test: registry backend\n");

	printf("test: init\n");
	werr = smbconf_init_reg(mem_ctx, &conf_ctx, NULL);
	if (!W_ERROR_IS_OK(werr)) {
		printf("failure: init failed: %s\n", dos_errstr(werr));
		ret = false;
		goto done;
	}
	printf("success: init\n");

	ret &= test_get_includes(conf_ctx);
	ret &= test_set_get_includes(conf_ctx);
	ret &= test_delete_includes(conf_ctx);

	smbconf_shutdown(conf_ctx);

	printf("%s: registry backend\n", ret ? "success" : "failure");

done:
	TALLOC_FREE(mem_ctx);
	return ret;
}

static bool torture_smbconf(void)
{
	bool ret = true;
	ret &= torture_smbconf_txt();
	printf("\n");
	ret &= torture_smbconf_reg();
	return ret;
}

int main(int argc, const char **argv)
{
	bool ret;
	poptContext pc;
	TALLOC_CTX *mem_ctx = talloc_stackframe();

	struct poptOption long_options[] = {
		POPT_COMMON_SAMBA
		{0, 0, 0, 0}
	};

	load_case_tables();
	dbf = x_stderr;

	/* parse options */
	pc = poptGetContext("smbconftort", argc, (const char **)argv,
			    long_options, 0);

	while(poptGetNextOpt(pc) != -1) { }

	poptFreeContext(pc);

	ret = lp_load(get_dyn_CONFIGFILE(),
		      true,  /* globals_only */
		      false, /* save_defaults */
		      false, /* add_ipc */
		      true   /* initialize globals */);

	if (!ret) {
		printf("failure: error loading the configuration\n");
		goto done;
	}

	ret = torture_smbconf();

done:
	TALLOC_FREE(mem_ctx);
	return ret ? 0 : -1;
}
