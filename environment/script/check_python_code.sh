set -euo pipefail

build_type=Debug
lue_build_prefix="$OBJECTS/$build_type/lue"
lue_python_prefix="$(echo "$lue_build_prefix"/lib*/python3.*)"

export PYTHONPATH="$lue_python_prefix:${PYTHONPATH:-}"

lue_ad_hoc_prefix="$lue_python_prefix/lue/command"

echo "ruff..."
ruff check "$lue_ad_hoc_prefix"
echo "pyright..."
pyright --project "$LUE" "$lue_ad_hoc_prefix"
echo "mypy..."
mypy --strict "$lue_ad_hoc_prefix"

# Figure out whether it is better to use typings/lue/data_model/__init__.pyi
# - https://peps.python.org/pep-0484/#stub-files
# - https://microsoft.github.io/pyright/#/type-stubs
# - https://github.com/microsoft/pyright/issues/10089
