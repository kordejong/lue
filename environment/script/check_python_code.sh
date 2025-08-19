set -euo pipefail

# TODO: We dump .pyi files in the subpackage directories. Figure out whether it is better to use
#   typings/lue/<subpackage>/__init__.pyi
# - https://peps.python.org/pep-0484/#stub-files
# - https://microsoft.github.io/pyright/#/type-stubs
# - https://github.com/microsoft/pyright/issues/10089

function parse_command_line() {
    usage="$(basename $0) build_prefix"

    if [[ $# != 1 ]]; then
        echo $usage
        exit 1
    fi

    lue_build_prefix=$1
}

function check_python_code() {
    lue_python_prefix="$(echo "$lue_build_prefix"/lib*/python3.*)"

    export PYTHONPATH="$lue_python_prefix:${PYTHONPATH:-}"

    # TODO: Add directories, test all installed Python code
    lue_ad_hoc_prefix="$lue_python_prefix/lue/command"

    echo "ruff..."
    ruff check "$lue_ad_hoc_prefix"
    echo "pyright..."
    pyright --project "$LUE" "$lue_ad_hoc_prefix"
    echo "mypy..."
    mypy --strict "$lue_ad_hoc_prefix"
}

parse_command_line $@
check_python_code
