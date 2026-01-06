#!/usr/bin/env python
import enum
import itertools
import logging
import math
import operator
import os
import random
import shutil
import sys
import traceback
import typing
from dataclasses import dataclass
from pathlib import Path
# import pprint

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


@dataclass
class ArgumentData:
    pcraster: dict[TYPE, dict[lpcr.VALUESCALE, typing.Any]]
    lue: dict[TYPE, dict[lpcr.VALUESCALE, typing.Any]]
    value: dict[lpcr.VALUESCALE, typing.Any]  #  = dict()

    def __init__(self):
        self.pcraster = {
            Spatial: dict(),
            NonSpatial: dict(),
        }
        self.lue = {
            Spatial: dict(),
            NonSpatial: dict(),
        }
        self.value = dict()
        raster_shape = (
            random.randrange(start=1, stop=100 + 1),
            random.randrange(start=1, stop=100 + 1),
        )
        cell_size = 10.0

        nr_rows, nr_cols = raster_shape
        north = 0.0
        west = 0.0
        east = west + nr_cols * cell_size
        south = north - nr_rows * cell_size

        pcr.setclone(nr_rows, nr_cols, cell_size, west, north)

        lpcr.configuration = lpcr.Configuration(
            bounding_box=lpcr.BoundingBox(
                north=north, west=west, south=south, east=east
            ),
            cell_size=cell_size,
            array_shape=raster_shape,
        )

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
        # TODO: Use numpy routines only to create a surface. Trigoniometry can help.
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
        self.lue[NonSpatial][lpcr.Nominal] = lfr.create_scalar(
            np.int32, self.value[lpcr.Nominal]
        )

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
        self.lue[NonSpatial][lpcr.Ordinal] = lfr.create_scalar(
            np.int32, self.value[lpcr.Ordinal]
        )

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
        self.lue[NonSpatial][lpcr.Scalar] = lfr.create_scalar(
            np.float32, self.value[lpcr.Scalar]
        )


# Global:
# - Value scale of result is fixed or same as value scale of argument
# - The cell representation and value scale of an argument are independent of those of other arguments
# Poly:
# - First input having more than one type determines result, if not yet fixed
# SameBin:
# - Binary operation
# - Argument value scales are the same
# - Result value scale is the same as the argument value scales
EXEC_RESULT = enum.Enum("EXEC_RESULT", ["ExecGlobal", "ExecPoly", "ExecSameBin"])
(ExecGlobal, ExecPoly, ExecSameBin) = EXEC_RESULT

SYNTAX = enum.Enum("SYNTAX", ["SyntaxFunction", "SyntaxOperator"])
(SyntaxFunction, SyntaxOperator) = SYNTAX

STATUS = enum.Enum("STATUS", ["OK", "NOT_OK"])


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
    syntax: SYNTAX

    def __str__(self) -> str:
        formatted_arguments = [f"{argument}" for argument in self.arguments]
        formatted_results = [f"{result}" for result in self.results]

        return f"{self.name}({', '.join(formatted_arguments)}) -> {', '.join(formatted_results)}"


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
    syntax: SYNTAX
    result_category: EXEC_RESULT

    def overload_arguments(
        self, potential_overload_arguments: list[tuple[OverloadArgument, ...]]
    ) -> list[tuple[OverloadArgument, ...]]:
        overload_arguments = []

        match self.result_category:
            case EXEC_RESULT.ExecGlobal:
                overload_arguments = potential_overload_arguments
            case EXEC_RESULT.ExecPoly | EXEC_RESULT.ExecSameBin:
                # Select those tuples of arguments whose value scale is the same
                overload_arguments = [
                    arguments
                    for arguments in potential_overload_arguments
                    if len(set(argument.value_scale for argument in arguments)) == 1
                ]

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
        """
        For each tuple of arguments, return a tuple of results
        """
        overload_results = []

        # if self.result_category == ExecGlobal:
        match self.result_category:
            case EXEC_RESULT.ExecGlobal:
                for arguments in overload_arguments:
                    # first_argument = arguments[0]

                    for results in potential_overload_results:
                        overload_results.append(tuple(results))
                        break
                        # for result in results:
                        #     if result.value_scale == first_argument.value_scale:
                        #         overload_results.append(tuple(results))
                        #         break
            case EXEC_RESULT.ExecPoly:
                assert len(overload_arguments) > 0

                for arguments in overload_arguments:
                    # first_argument = arguments[0]
                    # argument_value_scale = first_argument.value_scale
                    at_least_one_argument_is_spatial = any(
                        argument.type == TYPE.Spatial for argument in arguments
                    )
                    type_ = (
                        TYPE.Spatial
                        if at_least_one_argument_is_spatial
                        else TYPE.NonSpatial
                    )

                    for results in potential_overload_results:
                        result_value_scale_is_fixed = (
                            len(set(result.value_scale for result in results)) == 1
                        )
                        assert result_value_scale_is_fixed, self.name

                        for result in results:
                            if (
                                result.type == type_
                                # and result.value_scale == argument_value_scale
                            ):
                                overload_results.append(tuple(results))
                                break

                # print(f"operator: {self.name}")
                # print(f"    arguments: {pprint.pformat(overload_arguments)}")
                # print(f"      results: {pprint.pformat(overload_results)}")
                #
                # assert False
            case EXEC_RESULT.ExecSameBin:
                assert len(overload_arguments) > 0

                for arguments in overload_arguments:
                    assert len(arguments) == 2, f"{self.name}: {arguments}"

                    argument_value_scale_is_fixed = (
                        len(set(argument.value_scale for argument in arguments)) == 1
                    )
                    assert argument_value_scale_is_fixed, self.name
                    argument_value_scale = arguments[0].value_scale

                    at_least_one_argument_is_spatial = any(
                        argument.type == TYPE.Spatial for argument in arguments
                    )
                    type_ = (
                        TYPE.Spatial
                        if at_least_one_argument_is_spatial
                        else TYPE.NonSpatial
                    )

                    for results in potential_overload_results:
                        result_value_scale_is_fixed = (
                            len(set(result.value_scale for result in results)) == 1
                        )
                        assert result_value_scale_is_fixed, self.name
                        result_value_scale = results[0].value_scale

                        for result in results:
                            assert result_value_scale == argument_value_scale

                            if result.type == type_:
                                overload_results.append(tuple(results))
                                break

        # else:
        #     assert False, self.result_category

        assert len(overload_results) == len(overload_arguments)

        # assert overload_results, potential_overload_results

        return overload_results

    def overloads(self) -> list[Overload]:
        # For each overload a combination of arguments. This set is likely too large.
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
            Overload(self.name, arguments, results, self.syntax)
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
                assert value_scale in argument_data.pcraster[type], (
                    f"{type}: {value_scale}"
                )
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


pcr_unary_operator_name_to_python_operator_name = {
    "+": "pos",
    "-": "neg",
    "not": "invert",
}


pcr_binary_operator_name_to_python_operator_name = {
    "*": "mul",
    "**": "pow",
    "/": "truediv",
    "idiv": "floordiv",
    "+": "add",
    "-": "sub",
    "lt": "lt",
    "le": "le",
    "eq": "eq",
    "ne": "ne",
    "ge": "ge",
    "gt": "gt",
    "and": "and_",
    "or": "or_",
    "xor": "xor",
    "mod": "mod",
}


def call_pcraster_operation(
    overload: Overload,
    argument_data: ArgumentData,
    logger: logging.Logger,
    output_directory: Path,
) -> tuple[STATUS, tuple]:
    match overload.syntax:
        case SYNTAX.SyntaxFunction:
            function = getattr(pcr, overload.name)
        case SYNTAX.SyntaxOperator:
            match len(overload.arguments):
                case 1:
                    function = getattr(
                        operator,
                        pcr_unary_operator_name_to_python_operator_name[overload.name],
                    )
                case 2:
                    function = getattr(
                        operator,
                        pcr_binary_operator_name_to_python_operator_name[overload.name],
                    )
                case _:
                    assert False

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
    match overload.syntax:
        case SYNTAX.SyntaxFunction:
            function = getattr(lpcr, overload.name)
        case SYNTAX.SyntaxOperator:
            match len(overload.arguments):
                case 1:
                    function = getattr(
                        operator,
                        pcr_unary_operator_name_to_python_operator_name[overload.name],
                    )
                case 2:
                    function = getattr(
                        operator,
                        pcr_binary_operator_name_to_python_operator_name[overload.name],
                    )
                case _:
                    assert False

    arguments = [
        argument_data.lue[argument.type][argument.value_scale]
        for argument in overload.arguments
    ]

    # If the operation accepts a window length (focal operation), make sure it is a whole number of cell
    # sizes
    if overload.name.startswith("window") and overload.name != "window4total":
        assert overload.arguments[1].name in ["windowsize", "windowSize"], (
            overload.arguments[1]
        )
        arguments[1] = lpcr.configuration.cell_size

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
    match operation.syntax:
        case SYNTAX.SyntaxFunction:
            name = operation.name
        case SYNTAX.SyntaxOperator:
            match len(operation.arguments):
                case 1:
                    name = pcr_unary_operator_name_to_python_operator_name[
                        operation.name
                    ]
                case 2:
                    name = pcr_binary_operator_name_to_python_operator_name[
                        operation.name
                    ]
                case _:
                    assert False

    return Path(name)


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

    operation_status = STATUS.NOT_OK

    if operation.syntax == SyntaxFunction:
        assert hasattr(pcr, operation.name), operation.name

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
                        operation,
                        f"Number of overloads that failed: {nr_failed_overloads}",
                    )
                )
            else:
                operation_status = STATUS.OK
    elif operation.syntax == SyntaxOperator:
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
                    operation,
                    f"Number of overloads that failed: {nr_failed_overloads}",
                )
            )
        else:
            operation_status = STATUS.OK
    else:
        assert False, operation

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
    argument_data: ArgumentData,
    logger: logging.Logger,
    output_directory: Path,
    show_progress: bool,
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
        "extentofview",
        "gradx",
        "grady",
        "ibngauss",
        "inversedistance",
        "influencesimplegauss",
        "laplacian",
        "lax",
        "markwhilesumge",
        "markwhilesumle",
        "move",
        "plancurv",
        "profcurv",
        "riksfraction",
        "shift",
        "shift0",
        "squarefraction",
        "transient",
        "view",
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
                                SyntaxFunction,
                                ExecGlobal,
                            )
                        )
            else:
                logger.debug(f"Skipping operation {name}: exec {syntax}")
        elif syntax == "Operator":
            commutative = element.attrib["commutative"]

            assert exec in ["POLY", "SAME_BIN", "SAME_UN"], exec
            assert commutative, commutative

            match exec:
                case "POLY":
                    operations.append(
                        Operation(
                            name,
                            [element_to_argument(input) for input in inputs],
                            [element_to_result(result) for result in results],
                            SyntaxOperator,
                            ExecPoly,
                        )
                    )
                case "SAME_BIN":
                    operations.append(
                        Operation(
                            name,
                            [element_to_argument(input) for input in inputs],
                            [element_to_result(result) for result in results],
                            SyntaxOperator,
                            ExecSameBin,
                        )
                    )
                case _:
                    # TODO: Support all kinds of operators
                    # print(etree.tostring(element))
                    logger.debug(f"Skipping operation {name}: exec {syntax}")
        else:
            logger.debug(f"Skipping operation {name}: syntax {syntax}")

    nr_failed_operations = 0

    for i in tqdm(range(len(operations)), disable=not show_progress):
        status = verify_operation(
            operations[i], argument_data, logger, output_directory
        )

        if status != STATUS.OK:
            nr_failed_operations += 1

    logger.info(f"Number of operations that failed: {nr_failed_operations}")
    return STATUS.NOT_OK if nr_failed_operations > 0 else STATUS.OK


@lfr.runtime_scope
def verify_lue_pcraster(
    logger: logging.Logger, output_directory: Path, show_progress: bool
) -> None:
    """
    For each overload of each PCRaster operation, compare the outputs of lue.pcraster with PCRaster
    """
    initialize_output_directory(output_directory)

    argument_data = ArgumentData()

    verify_arguments(argument_data, logger)
    status = verify_operations(argument_data, logger, output_directory, show_progress)

    if status != STATUS.OK:
        write_arguments(argument_data, output_directory / "data")
        logger.info("The lue.pcraster subpackage is not ready yet")
    else:
        logger.info("The lue.pcraster subpackage is ready, given the tests performed")


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
    --debug     Turn on debug log level
    -h --help   Show this screen and exit
    --version   Show version and exit

PCRaster's operations.{{dtd,xml}} will be downloaded unless they are already
present in the output directory.

This script is intended to be used from a Conda environment with both LUE and
PCRaster packages installed. The folowing procedure can be used for convenient
change / build / test cycles of the LUE software:

- Make a change
- Build and test as usual
- Create a new LUE Conda package:
    - In a shell with Conda base environment containing conda-build (and
      conda-verify)
    - $ conda-build environment/conda --python=3.13 --numpy=2
- Test the new LUE Conda package:
    - In a shell with Conda environment containing lxml pcraster requests tqdm
    - $ conda install --force-reinstall --channel <prefix> --use-local lue
    - $ LD_PRELOAD=$(find $CONDA_PREFIX -name libtcmalloc_minimal.so.4) \
            source/framework/python/script/lue_verify_pcraster.py /tmp/blah

Notes:

- Make sure the Python versions in the Conda environments match.
"""
    arguments = sys.argv[1:]
    arguments = docopt.docopt(usage, arguments)
    logger = logging.getLogger(command)

    if arguments["--debug"]:
        logging.basicConfig(level=logging.DEBUG)
    else:
        logging.basicConfig(level=logging.INFO)

    output_directory = Path(arguments["<directory>"])

    show_progress = not arguments["--debug"]

    status = 1

    try:
        verify_lue_pcraster(logger, output_directory, show_progress)
        status = 0
    except Exception as exception:
        # This should never happen, except during development. Dump the trace.
        logger.error(exception)
        logger.error(traceback.format_exc())

    return status


if __name__ == "__main__":
    sys.exit(main())
