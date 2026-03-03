from pathlib import Path

import matplotlib.pyplot as plt
import rasterio
import rasterio.plot


# https://matplotlib.org/stable/users/explain/colors/colormaps.html
colour_map_by_value_scale = {
    "scalar": "plasma",
}


def value_scale(raster: rasterio.io.DatasetReader) -> str:
    assert len(raster.dtypes) == 1

    dtype = raster.dtypes[0]

    if dtype in ["float32"]:
        return "scalar"
    else:
        raise ValueError(f"Element type {dtype} is not supported yet")


def colour_map(raster: rasterio.io.DatasetReader) -> str:
    return colour_map_by_value_scale[value_scale(raster)]


def create_map(raster_path: Path, figure_path: Path) -> None:
    # https://py.geocompx.org
    raster = rasterio.open(raster_path)
    nr_rows, nr_cols = raster.shape
    figure, axes = plt.subplots(figsize=(nr_rows, nr_cols))

    # TODO: Style:
    #       - Show cell values (or legend?)
    rasterio.plot.show(raster, ax=axes, cmap=colour_map(raster))

    plt.savefig(figure_path, transparent=True)
