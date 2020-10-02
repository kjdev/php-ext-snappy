$ErrorActionPreference = "Stop"

Set-Location C:\projects\snappy

if ($env:VCPKG -eq '0') {
    $command = "git"
    $args = "submodule update --init --recursive"
    $process = Start-Process $command -ArgumentList $args -Wait -NoNewWindow -PassThru
    if ($process.ExitCode -ne 0) {
        Write-Error -Category InvalidResult -Message "$command exits with $($process.ExitCode)"
    }

    # Create "snappy\snappy-stubs-public.h"
    Get-Content "snappy\snappy-stubs-public.h.in" | ForEach-Object { $_ -replace '\${HAVE_SYS_UIO_H_01}', "0" } | Out-File "snappy\snappy-stubs-public.h"
}
