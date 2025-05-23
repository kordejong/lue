set -euo pipefail

build_type=Debug
lue_build_prefix="$OBJECTS/$build_type/lue"
lue_python_prefix="$(echo "$lue_build_prefix"/lib*/python3.*)"

export PYTHONPATH="$lue_python_prefix:${PYTHONPATH:-}"

lue_ad_hoc_prefix="$lue_python_prefix/lue/command"

echo "ruff..."
ruff check "$lue_ad_hoc_prefix"
echo "pyright..."
pyright --project $LUE "$lue_ad_hoc_prefix"
echo "mypy..."
mypy --check-untyped-defs "$lue_ad_hoc_prefix"

# TODO Create and install lib/python3.12/lue/data_model/__init__.pyi with this content:
# def __getattr__(name: str) -> Any: ...
#
# Figure out whether it is better to use typings/lue/data_model/__init__.pyi
# - https://peps.python.org/pep-0484/#stub-files
# - https://microsoft.github.io/pyright/#/type-stubs
# - https://github.com/microsoft/pyright/issues/10089
#
# TODO Make sure pyright and mypy are still in pre-commit
