from pathlib import Path

import matplotlib.pyplot as plt
import rasterio
import rasterio.plot


# https://matplotlib.org/stable/users/explain/colors/colormaps.html
colour_map_by_value_scale = {
    "boolean": "Dark2",
    "nominal": "Dark2",
    "ordinal": "viridis",
    "scalar": "viridis",
}


def default_value_scale(raster: rasterio.io.DatasetReader) -> str:
    assert raster.count == 1

    dtype = raster.dtypes[0]

    if dtype in ["uint8"]:
        return "boolean"

    if dtype in ["uint16", "uint32", "uint64", "int8", "int16", "int32", "int64"]:
        return "ordinal"

    if dtype in ["float32", "float64"]:
        return "scalar"

    raise ValueError(f"Element type {dtype} is not supported yet")


def colour_map(raster: rasterio.io.DatasetReader) -> str:
    value_scale = raster.tags(1, ns="lue").get(
        "value_scale", default_value_scale(raster)
    )

    return colour_map_by_value_scale[value_scale]


def create_map(raster_path: Path, figure_path: Path, formats: list[str]) -> None:
    # https://py.geocompx.org
    raster = rasterio.open(raster_path)
    nr_rows, nr_cols = raster.shape
    figure, axes = plt.subplots(figsize=(nr_rows, nr_cols))

    # TODO: Style:
    #       - Show cell values (or legend?)
    rasterio.plot.show(raster, ax=axes, cmap=colour_map(raster))

    for format in formats:
        plt.savefig(figure_path.with_suffix(f".{format}"), transparent=True)
