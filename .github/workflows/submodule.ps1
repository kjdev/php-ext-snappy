
Invoke-Expression "git submodule update --init --recursive"

# Create "snappy\snappy-stubs-public.h"
Get-Content "snappy\snappy-stubs-public.h.in" | ForEach-Object { $_ -replace '\${HAVE_SYS_UIO_H_01}', "0" } | Out-File "snappy\snappy-stubs-public.h"
