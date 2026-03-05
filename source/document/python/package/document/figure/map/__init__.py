import math
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


def skip(value, no_data_value) -> bool:
    if math.isnan(no_data_value):
        return math.isnan(value)
    else:
        return value == no_data_value


def create_map(raster_path: Path, figure_path: Path, formats: list[str]) -> None:
    # https://py.geocompx.org
    raster = rasterio.open(raster_path)
    nr_rows, nr_cols = raster.shape
    figure, axes = plt.subplots(figsize=(nr_rows, nr_cols))

    rasterio.plot.show(raster, ax=axes, cmap=colour_map(raster))
    axes.set_axis_off()

    nodata_value = raster.nodata
    array = raster.read(1)
    x_coordinates, y_coordinates, cell_values = [], [], []
    coordinates = []

    for row in range(nr_rows):
        for col in range(nr_cols):
            x_coordinate, y_coordinate = raster.xy(row, col)
            coordinates.append(raster.xy(row, col))
            cell_value = array[row, col]

            x_coordinates.append(x_coordinate)
            y_coordinates.append(y_coordinate)
            cell_values.append(cell_value)

    for idx, coordinate in enumerate(coordinates):
        if not skip(cell_values[idx], nodata_value):
            axes.annotate(
                f"{cell_values[idx]}",
                xy=coordinate,
                textcoords="data",
                ha="center",
                va="center",
            )

    for format in formats:
        plt.savefig(figure_path.with_suffix(f".{format}"), transparent=True)
