from pathlib import Path

from .map import create_map


def plot(dataset_path: Path, figure_path: Path) -> None:

    if dataset_path.suffix in [".tif"]:
        create_map(dataset_path, figure_path)
    else:
        raise ValueError(f"Don't know how to plot {dataset_path} yet")
