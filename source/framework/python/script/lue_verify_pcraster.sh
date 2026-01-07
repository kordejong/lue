#!/usr/bin/env bash
set -eu

# Example runner to do the verification.

# NOTE:Cache $HOME/tmp/operation.{dtd,xml} in $HOME/tmp or adjust script

output_prefix=/tmp/lue_verify_pcraster

if [[ ! -d $output_prefix ]]; then
    mkdir -p $output_prefix
else
    rm -r ${output_prefix:?}/*
fi

# Use cached operation.{dtd,xml} to speed up things
cp "$HOME/tmp/operation."{dtd,xml} $output_prefix

LD_PRELOAD=$(find "$CONDA_PREFIX" -name libtcmalloc_minimal.so.4) \
    source/framework/python/script/lue_verify_pcraster.py $output_prefix
