#!/usr/bin/env python
import enum
import itertools
import logging
import math
import os
import random
import shutil
import sys
import traceback
import typing
from dataclasses import dataclass
from pathlib import Path

import docopt
import numpy as np
import pcraster as pcr
import requests

# from xml import Element
# from xml.etree import ElementTree
from lxml import etree
from tqdm import tqdm

import lue.framework as lfr
import lue.pcraster as lpcr


def initialize_output_directory(output_directory: Path) -> None:
    # if output_directory.is_dir():
    #     raise RuntimeError(
    #         f"Output directory ({output_directory}) must not already exist. Remove it first."
    #     )
    output_directory.mkdir(exist_ok=True)


TYPE = enum.Enum("TYPE", ["Spatial", "NonSpatial"])
(Spatial, NonSpatial) = TYPE


# @dataclass
# class PCRasterArguments:
#     spatial = dict()
#     non_spatial = dict()
#         # argument_data.pcraster[argument.type][argument.value_scale]


# @dataclass
# class LUEArguments:
#     array = dict()
#     scalar = dict()


@dataclass
class ArgumentData:
    # pcraster = PCRasterArguments()
    # lue = LUEArguments()
    pcraster: dict[TYPE, dict[lpcr.VALUESCALE, typing.Any]]
    lue: dict[TYPE, dict[lpcr.VALUESCALE, typing.Any]]
    value: dict[lpcr.VALUESCALE, typing.Any] = dict()

    def __init__(self):
        self.pcraster = {
            Spatial: dict(),
            NonSpatial: dict(),
        }
        self.lue = {
            Spatial: dict(),
            NonSpatial: dict(),
        }
        raster_shape = (
            random.randrange(start=1, stop=100 + 1),
            random.randrange(start=1, stop=100 + 1),
        )
        cell_size = 10.0

        pcr.setclone(*raster_shape, cell_size, 0.0, 0.0)
        generator = np.random.default_rng()

        self.add_boolean(generator, raster_shape)
        self.add_ldd(generator, raster_shape)
        self.add_nominal(generator, raster_shape)
        self.add_ordinal(generator, raster_shape)
        self.add_scalar(generator, raster_shape)

        # NOTE: Directional?

    def add_boolean(
        self, generator: np.random.Generator, raster_shape: tuple[int, int]
    ) -> None:
        # Boolean values in range [0, 2] where 2 will result in a no-data value
        array = (
            generator.integers(
                low=0, high=2, size=math.prod(raster_shape), endpoint=True
            )
            .astype(dtype=np.uint8)
            .reshape(*raster_shape)
        )
        self.value[lpcr.Boolean] = random.randrange(start=0, stop=2 + 1)
        assert isinstance(self.value[lpcr.Boolean], int), type(self.value[lpcr.Boolean])

        self.pcraster[Spatial][lpcr.Boolean] = pcr.numpy2pcr(
            dataType=pcr.Boolean, array=array, mv=2
        )
        self.pcraster[NonSpatial][lpcr.Boolean] = pcr.boolean(self.value[lpcr.Boolean])

        # TODO: Arguments of LUE's version must be named the same as the PCRaster version
        self.lue[Spatial][lpcr.Boolean] = lpcr.numpy2pcr(
            lpcr.Boolean, array, no_data_value=2
        )
        self.lue[NonSpatial][lpcr.Boolean] = lpcr.boolean(self.value[lpcr.Boolean])

    def add_ldd(
        self, generator: np.random.Generator, raster_shape: tuple[int, int]
    ) -> None:
        # TODO: Use numpy routines only to create a surface. Trigoniomety can help.
        pass

    def add_nominal(
        self, generator: np.random.Generator, raster_shape: tuple[int, int]
    ) -> None:
        # Integral values in range [0, 20] where ≥ 17 will result in a no-data value
        array = (
            generator.integers(
                low=0, high=20, size=math.prod(raster_shape), endpoint=True
            )
            .astype(dtype=np.int32)
            .reshape(*raster_shape)
        )
        array = np.where(array < 17, array, 20)

        self.value[lpcr.Nominal] = random.randrange(start=0, stop=20 + 1)
        assert isinstance(self.value[lpcr.Nominal], int), type(self.value[lpcr.Nominal])

        self.pcraster[Spatial][lpcr.Nominal] = pcr.numpy2pcr(
            dataType=pcr.Nominal, array=array, mv=20
        )
        self.pcraster[NonSpatial][lpcr.Nominal] = pcr.nominal(self.value[lpcr.Nominal])

        self.lue[Spatial][lpcr.Nominal] = lpcr.numpy2pcr(
            lpcr.Nominal, array, no_data_value=20
        )
        self.lue[NonSpatial][lpcr.Nominal] = lpcr.scalar(self.value[lpcr.Nominal])

    def add_ordinal(
        self, generator: np.random.Generator, raster_shape: tuple[int, int]
    ) -> None:
        # Integral values in range [0, 20] where ≥ 17 will result in a no-data value
        array = (
            generator.integers(
                low=0, high=20, size=math.prod(raster_shape), endpoint=True
            )
            .astype(dtype=np.int32)
            .reshape(*raster_shape)
        )
        array = np.where(array < 17, array, 20)

        self.value[lpcr.Ordinal] = random.randrange(start=0, stop=20 + 1)
        assert isinstance(self.value[lpcr.Ordinal], int), type(self.value[lpcr.Ordinal])

        self.pcraster[Spatial][lpcr.Ordinal] = pcr.numpy2pcr(
            dataType=pcr.Ordinal, array=array, mv=20
        )
        self.pcraster[NonSpatial][lpcr.Ordinal] = pcr.ordinal(self.value[lpcr.Ordinal])

        self.lue[Spatial][lpcr.Ordinal] = lpcr.numpy2pcr(
            lpcr.Ordinal, array, no_data_value=20
        )
        self.lue[NonSpatial][lpcr.Ordinal] = lpcr.scalar(self.value[lpcr.Ordinal])

    def add_scalar(
        self, generator: np.random.Generator, raster_shape: tuple[int, int]
    ) -> None:
        # Floating point values in range [0, 1000) where ≥ 950 will result in a no-data value
        array = (
            generator.uniform(low=0, high=1000, size=math.prod(raster_shape))
            .astype(dtype=np.float32)
            .reshape(*raster_shape)
        )
        array = np.where(array < 950, array, 1000)

        self.value[lpcr.Scalar] = generator.uniform(low=0, high=1000)
        assert isinstance(self.value[lpcr.Scalar], float), type(self.value[lpcr.Scalar])

        self.pcraster[Spatial][lpcr.Scalar] = pcr.numpy2pcr(
            dataType=pcr.Scalar, array=array, mv=1000
        )
        self.pcraster[NonSpatial][lpcr.Scalar] = pcr.scalar(self.value[lpcr.Scalar])

        self.lue[Spatial][lpcr.Scalar] = lpcr.numpy2pcr(
            lpcr.Scalar, array, no_data_value=1000.0
        )
        self.lue[NonSpatial][lpcr.Scalar] = lpcr.scalar(self.value[lpcr.Scalar])


@dataclass
class OverloadArgument:
    name: str
    type: TYPE
    value_scale: lpcr.VALUESCALE
    variadic: bool

    def __str__(self) -> str:
        return f"{self.name} [{self.type.name}, {self.value_scale.name}]"


@dataclass
class OverloadResult:
    type: TYPE
    value_scale: lpcr.VALUESCALE

    def __str__(self) -> str:
        return f"[{self.type.name}, {self.value_scale.name}]"


@dataclass
class Overload:
    name: str
    arguments: tuple[OverloadArgument, ...]
    results: tuple[OverloadResult, ...]

    def __str__(self) -> str:
        formatted_arguments = [f"{argument}" for argument in self.arguments]
        formatted_results = [f"{result}" for result in self.results]

        return f"{self.name}({', '.join(formatted_arguments)}) -> {', '.join(formatted_results)}"


# Global:
# - Value scale of result is fixed or same as value scale of argument
# - The cell representation and value scale of an argument are independent of those of other arguments
EXEC_RESULT = enum.Enum("EXEC_RESULT", ["ExecGlobal"])
(ExecGlobal,) = EXEC_RESULT


STATUS = enum.Enum("STATUS", ["OK", "NOT_OK"])


@dataclass
class OperationArgument:
    name: str
    types: set[TYPE]
    value_scales: set[lpcr.VALUESCALE]
    variadic: bool
    """
    Each argument contains information about the supported cell representations and value scales. A valid
    argument is an argument having a supported cell representation and value scale.
    """


@dataclass
class OperationResult:
    types: set[TYPE]
    value_scales: set[lpcr.VALUESCALE]
    """
    Each result contains information about the supported cell representations and value scales. Which
    of these will be used depends on the types and values scales of arguments passed in and on the execution
    result category.
    """


@dataclass
class Operation:
    name: str
    arguments: list[OperationArgument]
    results: list[OperationResult]
    result_category: EXEC_RESULT

    def overload_arguments(
        self, potential_overload_arguments: list[tuple[OverloadArgument, ...]]
    ) -> list[tuple[OverloadArgument, ...]]:
        overload_arguments = []

        # TODO: Generalize, based on ...
        overload_arguments = potential_overload_arguments

        # # Skip overloads taking a directional argument. LUE doesn't do directionals
        # overload_arguments = [
        #     arguments
        #     for arguments in potential_overload_arguments
        #     if not any(
        #         argument.value_scale in (lpcr.Directional, lpcr.Ldd)
        #         for argument in arguments
        #     )
        # ]

        assert overload_arguments

        return overload_arguments

    def overload_results(
        self,
        potential_overload_results: list[tuple[OverloadResult, ...]],
        overload_arguments: list[tuple[OverloadArgument, ...]],
    ) -> list[tuple[OverloadResult, ...]]:
        overload_results = []

        if self.result_category == ExecGlobal:
            for arguments in overload_arguments:
                first_argument = arguments[0]

                for results in potential_overload_results:
                    overload_results.append(tuple(results))
                    break
                    # for result in results:
                    #     if result.value_scale == first_argument.value_scale:
                    #         overload_results.append(tuple(results))
                    #         break
        else:
            assert False, self.result_category

        assert overload_results, potential_overload_results

        return overload_results

    def overloads(self) -> list[Overload]:
        # For each overload a combination of arguments
        potential_overload_arguments = list(
            itertools.product(
                *tuple(
                    tuple(
                        OverloadArgument(
                            argument.name,
                            *product,
                            argument.variadic,
                        )
                        for product in itertools.product(
                            argument.types, argument.value_scales
                        )
                    )
                    for argument in self.arguments
                )
            )
        )
        overload_arguments = self.overload_arguments(potential_overload_arguments)

        potential_overload_results = list(
            itertools.product(
                *tuple(
                    tuple(
                        OverloadResult(
                            *product,
                        )
                        for product in itertools.product(
                            result.types, result.value_scales
                        )
                    )
                    for result in self.results
                )
            )
        )
        overload_results = self.overload_results(
            potential_overload_results, overload_arguments
        )

        assert len(overload_arguments) == len(overload_results)

        overloads = [
            Overload(self.name, arguments, results)
            for arguments, results in zip(overload_arguments, overload_results)
        ]

        return overloads


def format_operation_message(operation: Operation, message: str = "") -> str:
    return f"Operation {operation.name}" + (f": {message}" if message else "")


def format_overload_message(overload: Overload, message: str = "") -> str:
    return f"{overload}" + (f": {message}" if message else "")


def write_arguments(argument_data: ArgumentData, output_directory: Path) -> None:
    if output_directory.exists():
        return

    output_directory.mkdir()

    pcraster_output_directory = output_directory / "pcraster"
    pcraster_output_directory.mkdir()

    lue_output_directory = output_directory / "lue"
    lue_output_directory.mkdir()

    for value_scale, name in [
        (lpcr.Boolean, "boolean"),
        (lpcr.Nominal, "nominal"),
        (lpcr.Ordinal, "ordinal"),
        (lpcr.Scalar, "scalar"),
    ]:
        pcr.report(
            argument_data.pcraster[Spatial][value_scale],
            str(pcraster_output_directory / f"{name}.map"),
        )

        lpcr.setclone(str(pcraster_output_directory / f"{name}.map"))

        lpcr.report(
            argument_data.lue[Spatial][value_scale],
            str(lue_output_directory / f"{name}.tif"),
        )

    assert output_directory.is_dir()


def verify_arguments(argument_data: ArgumentData, logger: logging.Logger) -> None:
    """
    Verify the collection of PCRaster and LUE input arguments is good

    Otherwise all bets are off of course
    """
    for type in TYPE:
        assert type in argument_data.pcraster, type
        assert type in argument_data.lue, type

        for value_scale in lpcr.VALUESCALE:
            # TODO: Support Ldd too
            if value_scale not in (lpcr.Directional, lpcr.Ldd):
                assert (
                    value_scale in argument_data.pcraster[type]
                ), f"{type}: {value_scale}"
                assert value_scale in argument_data.lue[type], f"{type}: {value_scale}"


def call_operation(function, arguments, logger: logging.Logger) -> tuple[STATUS, tuple]:
    status = STATUS.NOT_OK

    try:
        results = function(*arguments)
        if not isinstance(results, tuple):
            results = (results,)
        status = STATUS.OK
    except Exception as exception:
        logger.info(traceback.format_exc())
        results = tuple()

    return status, results


def call_pcraster_operation(
    overload: Overload,
    argument_data: ArgumentData,
    logger: logging.Logger,
    output_directory: Path,
) -> tuple[STATUS, tuple]:
    function = getattr(pcr, overload.name)
    arguments = [
        argument_data.pcraster[argument.type][argument.value_scale]
        for argument in overload.arguments
    ]

    status, results = call_operation(function, arguments, logger)

    if status != STATUS.OK:
        # This should not happen...
        for idx, result in enumerate(results):
            pcr.report(result, str(output_directory / f"result-{idx}.map"))

        logger.info(
            format_overload_message(
                overload,
                "Error while calling PCRaster operation. See error message before this line.",
            )
        )

    return status, results


def call_lue_operation(
    overload: Overload,
    argument_data: ArgumentData,
    logger: logging.Logger,
    output_directory: Path,
) -> tuple[STATUS, tuple]:
    function = getattr(lpcr, overload.name)
    arguments = [
        argument_data.lue[argument.type][argument.value_scale]
        for argument in overload.arguments
    ]

    status, results = call_operation(function, arguments, logger)

    if status != STATUS.OK:
        for idx, result in enumerate(results):
            # TODO: Support writing non-spatials
            if lpcr.is_spatial(result):
                lpcr.report(result, str(output_directory / f"result-{idx}.tif"))

            logger.info(
                format_overload_message(
                    overload,
                    "Error while calling LUE operation. See error message before this line.",
                )
            )

    return status, results


def compare_results(
    pcraster_results: tuple,
    pcraster_result_directory: Path,
    lue_results: tuple,
    lue_result_directory: Path,
) -> STATUS:
    # TODO: Implement
    return STATUS.OK


def operation_as_path(operation: Operation) -> Path:
    return Path(operation.name)


def overload_as_path(overload: Overload) -> Path:
    formatted_arguments = [
        f"{argument.name}[{argument.type.name},{argument.value_scale.name}]"
        for argument in overload.arguments
    ]
    # return Path(f"{overload.name}/{','.join(formatted_arguments)}")
    return Path(",".join(formatted_arguments))


def verify_overload(
    overload: Overload,
    argument_data: ArgumentData,
    logger: logging.Logger,
    output_directory: Path,
) -> STATUS:
    """
    Verify the results of calling a PCRaster and equivalent LUE operation overload are equal
    """
    # TODO: Verify the arguments are named the same / correctly. Note that there is no relation between the
    # names of the arguments in the XML and the names used in the API.
    # pcraster_function = getattr(pcr, overload.name)
    # lue_function = getattr(lpcr, overload.name)

    status = STATUS.NOT_OK

    overload_output_directory = output_directory / overload_as_path(overload)
    overload_output_directory.mkdir()

    pcraster_output_directory = overload_output_directory / "pcraster"
    pcraster_output_directory.mkdir()

    pcraster_status, pcraster_results = call_pcraster_operation(
        overload, argument_data, logger, pcraster_output_directory
    )

    lue_output_directory = overload_output_directory / "lue"
    lue_output_directory.mkdir()

    lue_status, lue_results = call_lue_operation(
        overload, argument_data, logger, lue_output_directory
    )

    if pcraster_status == STATUS.OK and lue_status == STATUS.OK:
        status = compare_results(
            pcraster_results,
            pcraster_output_directory,
            lue_results,
            lue_output_directory,
        )

        if status != STATUS.OK:
            logger.info(
                format_overload_message(
                    overload,
                    "Results of PCRaster and LUE operations are different. "
                    "See error message before this line.",
                )
            )
        else:
            # All is fine, clean-up
            shutil.rmtree(overload_output_directory)
            status = STATUS.OK

    # logger.debug(format_overload_message(overload))
    return status


def verify_operation(
    operation: Operation,
    argument_data: ArgumentData,
    logger: logging.Logger,
    output_directory: Path,
) -> STATUS:
    """
    Verify the results of calling a PCRaster and equivalent LUE operation are equal

    All overloads are tested.
    """
    # TODO: Fill and return a log
    # - Failure: info about the operation that failed, including the arguments. Create a directory with input
    #   arguments (once, to share by all error cases), and PCRaster and LUE results (once per failing
    #   operation). Write script snippets.
    logger.debug(format_operation_message(operation))

    operation_output_directory = output_directory / operation_as_path(operation)
    operation_output_directory.mkdir()

    assert hasattr(pcr, operation.name), operation.name

    operation_status = STATUS.NOT_OK

    if not hasattr(lpcr, operation.name):
        logger.info(f"Operation {operation.name} is not available in lue.pcraster")
    else:
        nr_failed_overloads = 0

        for overload in operation.overloads():
            # TODO: Support Ldd
            if not any(
                argument.value_scale in [lpcr.Directional, lpcr.Ldd]
                for argument in overload.arguments
            ):
                overload_status = verify_overload(
                    overload, argument_data, logger, operation_output_directory
                )

                if overload_status != STATUS.OK:
                    nr_failed_overloads += 1

        if nr_failed_overloads > 0:
            logger.info(
                format_operation_message(
                    operation, f"Number of overloads that failed: {nr_failed_overloads}"
                )
            )
        else:
            operation_status = STATUS.OK

    if not os.listdir(operation_output_directory):
        # All is fine, clean-up
        operation_output_directory.rmdir()

    return operation_status


def download_operation_dtd_and_xml(output_directory: Path) -> None:
    for filename in ["operation.dtd", "operation.xml"]:
        file_path = output_directory / filename

        if not file_path.exists():
            file_url = f"https://raw.githubusercontent.com/pcraster/pcraster/refs/heads/master/source/pcraster_model_engine/{filename}"
            response = requests.get(file_url)
            response.raise_for_status()

            with open(file_path, "w") as file:
                file.write(response.text)


def read_operation_xml(output_directory: Path) -> etree.Element:
    """
    Return the contents of PCRaster's operation XML

    The file will be downloaded if it does not already exist
    """
    download_operation_dtd_and_xml(output_directory)
    xml_path = output_directory / "operation.xml"

    assert xml_path.exists(), xml_path

    parser = etree.XMLParser(dtd_validation=True)

    return etree.parse(xml_path, parser=parser).getroot()


def string_to_value_scale(string: str) -> lpcr.VALUESCALE:
    value_scale_by_string = {
        "Boolean": lpcr.Boolean,
        "Directional": lpcr.Directional,
        "Ldd": lpcr.Ldd,
        "Ordinal": lpcr.Ordinal,
        "Nominal": lpcr.Nominal,
        "Scalar": lpcr.Scalar,
    }

    return value_scale_by_string[string]


def field_types(element: etree.Element) -> set[TYPE]:
    return set(
        [Spatial]
        if element.attrib["spatial"] == "Yes"
        else (
            [NonSpatial]
            if element.attrib["spatial"] == "Non"
            else [NonSpatial, Spatial]
        )
    )


def field_value_scales(element: etree.Element) -> set[lpcr.VALUESCALE]:
    return set(
        [
            string_to_value_scale(data_type_element.attrib["value"])
            for data_type_element in element.findall("DataType")
        ]
    )


def element_to_argument(element: etree.Element) -> OperationArgument:
    assert len(element.attrib) <= 2, element.attrib
    assert all(
        attribute in ["contextName", "repeat"] for attribute in element.attrib
    ), element.attrib.keys()

    name = element.attrib["contextName"] if "contextName" in element.attrib else ""
    variadic = (
        element.attrib["repeat"] == "true" if "repeat" in element.attrib else False
    )

    field_element = element.find("Field")
    assert field_element is not None

    assert (
        ("spatial" in field_element.attrib)
        and (field_element.attrib["spatial"] in ["Yes", "Non"])
    ) or (len(field_element.attrib) == 0), field_element.attrib

    return OperationArgument(
        name,
        field_types(field_element),
        field_value_scales(field_element),
        variadic,
    )


def element_to_result(element: etree.Element) -> OperationResult:
    field_element = element.find("Field")

    return OperationResult(
        field_types(field_element),
        field_value_scales(field_element),
    )


def verify_operations(
    argument_data: ArgumentData, logger: logging.Logger, output_directory: Path
) -> STATUS:
    operations_root = read_operation_xml(output_directory)
    operations: list[Operation] = []

    operations_to_skip = [
        "accutraveltimefractionremoved",
        "brenner",
        "distributesimplegauss",
        "diver",
        "divergence",
        "drain",
        "ellipseaverage",
        "gradx",
        "grady",
        "ibngauss",
        "laplacian",
        "lax",
        "move",
        "riksfraction",
        "squarefraction",
    ]

    for element in operations_root.findall("Operation"):
        name = element.attrib["name"]
        syntax = element.attrib["syntax"]
        exec = element.attrib["exec"]
        inputs = element.findall("Input")
        results = element.findall("Result")

        assert syntax in ["Function", "MRF", "None", "Operator"], syntax
        assert exec in [
            "SAME_UN",
            "SAME_BIN",
            "DIFF_UN",
            "DIFF_BIN",
            "POLY",
            "IFTHENELSE",
            "IFTHEN",
            "Direct",
            "GEN_NS",
            "GEN_SP",
            "GLOBAL",
            "TOUT",
            "EXTERN",
            "TRIG",
            "CONV",
            "T_IN",
            "INDEX",
            "DOUBLE",
            "MRF",
            "MISC",
        ], exec

        if syntax == "Function":
            if exec == "GLOBAL":
                if any(input.find("Field") is None for input in inputs):
                    logger.debug(f"Skipping operation {name}: non-field input")
                elif any(result.find("Field") is None for result in results):
                    logger.debug(f"Skipping operation {name}: non-field result")
                else:
                    if name in operations_to_skip:
                        logger.debug(
                            f"Skipping operation {name}: no equivalent in lue.framework"
                        )
                    else:
                        # print(etree.tostring(element))
                        operations.append(
                            Operation(
                                name,
                                [element_to_argument(input) for input in inputs],
                                [element_to_result(result) for result in results],
                                ExecGlobal,
                            )
                        )
            else:
                logger.debug(f"Skipping operation {name}: exec {syntax}")
        else:
            logger.debug(f"Skipping operation {name}: syntax {syntax}")

    nr_failed_operations = 0

    for i in tqdm(range(len(operations))):
        status = verify_operation(
            operations[i], argument_data, logger, output_directory
        )

        if status != STATUS.OK:
            nr_failed_operations += 1

    logger.info(f"Number of operations that failed: {nr_failed_operations}")
    return STATUS.NOT_OK if nr_failed_operations > 0 else STATUS.OK


@lfr.runtime_scope
def verify_lue_pcraster(logger: logging.Logger, output_directory: Path) -> None:
    """
    For each overload of each PCRaster operation, compare the outputs of lue.pcraster with PCRaster
    """
    initialize_output_directory(output_directory)

    argument_data = ArgumentData()

    verify_arguments(argument_data, logger)
    status = verify_operations(argument_data, logger, output_directory)

    if status != STATUS.OK:
        write_arguments(argument_data, output_directory / "data")
        logger.info("The lue.pcraster subpackage is not ready yet")


def main() -> int:
    command = Path(sys.argv[0]).name
    usage = f"""\
Verify LUE's lue.pcraster subpackage behaves as PCRaster

Usage:
    {command} [options] <directory>

Options:
    directory   Directory to store results in. If it exists, then it is assumed
                that its contents can be overwritten. To prevent errors, make
                this an (almost) empty directory.
    -h --help   Show this screen and exit
    --version   Show version and exit

PCRaster's operations.{{dtd,xml}} will be downloaded unless they are already
present in the output directory.
"""
    arguments = sys.argv[1:]
    arguments = docopt.docopt(usage, arguments)
    logger = logging.getLogger(command)
    # logging.basicConfig(level=logging.DEBUG)
    logging.basicConfig(level=logging.INFO)
    # logging.basicConfig(level=logging.WARNING)
    # logging.basicConfig(level=logging.ERROR)

    output_directory = Path(arguments["<directory>"])

    status = 1

    try:
        verify_lue_pcraster(logger, output_directory)
        status = 0
    except Exception as exception:
        logger.error(exception)
        # # This should never happen. Dump the trace.
        # logger.error(traceback.format_exc())

    return status


if __name__ == "__main__":
    sys.exit(main())
