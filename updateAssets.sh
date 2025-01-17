#!/bin/bash

git pull
git subtree pull --prefix=assets/ . "origin/rtype_assets" --squash
