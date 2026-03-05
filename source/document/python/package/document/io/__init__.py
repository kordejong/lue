from pathlib import Path

import lue.framework as lfr


__all__ = ["read_array", "read_kernel"]


def read_array(path: Path):
    return lfr.from_gdal(str(path))


def read_kernel(path: Path):
    return lfr.to_numpy(lfr.from_gdal(str(path)))
