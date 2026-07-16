import sys
from pathlib import Path

import docopt

import lue.framework_x as lfrx


__all__ = ["main"]


Shape = tuple[int, int]


def parse_shape(string: str) -> Shape:
    string = string.strip("[]")
    shape = tuple(int(extent) for extent in string.split(","))
    assert len(shape) == 2, shape
    return shape[0], shape[1]


def parse_command_line() -> dict:
    usage = """\
Run a model

Usage:
    {command} [--partition_shape=<shape>] <array_shape> <nr_time_steps> <rate_limit>
""".format(command=Path(sys.argv[0]).name)

    argv = [arg for arg in sys.argv[1:] if not arg.startswith("--hpx")]
    arguments = docopt.docopt(usage, argv)

    array_shape = parse_shape(arguments["<array_shape>"])
    nr_time_steps = int(arguments["<nr_time_steps>"])
    rate_limit = int(arguments["<rate_limit>"])

    assert nr_time_steps >= 0, nr_time_steps

    partition_shape = arguments["--partition_shape"]

    if partition_shape is not None:
        partition_shape = parse_shape(partition_shape)

    return {
        "array_shape": array_shape,
        "partition_shape": partition_shape,
        "nr_time_steps": nr_time_steps,
        "rate_limit": rate_limit,
    }


@lfrx.runtime_scope
def run_model(
    Model,
    *,
    array_shape: Shape,
    partition_shape: Shape | None,
    nr_time_steps: int,
    rate_limit: int,
) -> None:

    model = Model(
        array_shape=array_shape,
        partition_shape=partition_shape,
    )

    # model.initialize()
    #
    # for time_step in range(nr_time_steps):
    #     state = model.simulate(time_step)
    #     # state.get()

    lfrx.run_deterministic(
        model,
        lfrx.DefaultProgressor(),
        nr_time_steps=nr_time_steps,
        rate_limit=rate_limit,
    )


def main(Model):
    arguments = parse_command_line()
    run_model(Model, **arguments)
