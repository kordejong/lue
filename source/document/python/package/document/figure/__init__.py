from pathlib import Path

from .map import create_map


def plot(dataset_path: Path, figure_path: Path, formats: list[str]) -> None:

    if dataset_path.suffix in [".tif"]:
        create_map(dataset_path, figure_path, formats)
    else:
        raise ValueError(f"Don't know how to plot {dataset_path} yet")
