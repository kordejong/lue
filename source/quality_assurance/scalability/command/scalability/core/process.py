import shlex
import subprocess
import tempfile


def execute_command(command: str) -> None:
    """
    Execute `command` and raise a :py:class:`RuntimeError` in case the result is not zero
    """
    result = subprocess.call(shlex.split(command))

    if result != 0:
        raise RuntimeError(f"Error executing {command}")


def lue_translate() -> str:
    """
    Return the name of LUE's translate command
    """
    return "lue_translate"


def import_lue_json(lue_json_pathname: str, lue_dataset_pathname: str) -> None:
    """
    Import a LUE JSON file into a LUE dataset
    """
    command = (
        f"{lue_translate()} import --add {lue_dataset_pathname} {lue_json_pathname}"
    )

    execute_command(command)


def create_dot_graph(lue_dataset_pathname: str, pdf_graph_pathname: str) -> None:
    """
    Create a dot graph of the LUE file containing the experiment results
    """
    with tempfile.NamedTemporaryFile(suffix=".json") as dot_properties_file:
        dot_properties_file.write(
            b"""\
{
    "dot": {
        "node": {
            "fontcolor": "white"
        }
    },

    "hdf5": {
        "group": {
            "shape": "oval"
        },
        "dataset": {
            "shape": "box"
        }
    },

    "lue": {
        "phenomenon": {
            "fillcolor": "#791d72"
        },
        "property_set": {
            "fillcolor": "#9a0000"
        },
        "object_tracker": {
            "fillcolor": "#f08000",
            "show": false
        },
        "property": {
            "fillcolor": "#419702"
        },
        "time_domain": {
            "fillcolor": "#36a2c9",
            "show_details": false
        },
        "space_domain": {
            "fillcolor": "#36a2c9",
            "show_details": false
        },
        "value": {
            "fillcolor": "#f08000",
            "show": false,
            "show_details": true
        }
    }
}
"""
        )
        dot_properties_file.seek(0)

        with tempfile.NamedTemporaryFile(suffix=".dot") as dot_graph_file:
            commands = [
                f"{lue_translate()} export --meta {dot_properties_file.name} {lue_dataset_pathname} {dot_graph_file.name}",
                f"dot -Tpdf -o {pdf_graph_pathname} {dot_graph_file.name}",
            ]

            for command in commands:
                execute_command(command)
