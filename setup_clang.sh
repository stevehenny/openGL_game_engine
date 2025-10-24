#!/usr/bin/env bash

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  cp .clangd_linux .clangd
elif [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "cygwin" ]]; then
  cp .clangd_windows .clangd
else
  echo "Unknown OS: $OSTYPE"
  exit 1
fi

echo "Configured .clangd for $OSTYPE"
