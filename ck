#!/usr/bin/env -S uv run -s
# /// script
# requires-python = ">=3.11"
# dependencies = [
#   "cutekit @ git+https://github.com/cute-engineering/cutekit.git@kbd-port",
# ]
# ///

import sys
import cutekit as ck

sys.exit(ck.main())