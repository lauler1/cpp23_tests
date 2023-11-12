#!/bin/bash

# Try to get the latest tag; if it fails, default to "No Tag"
TAG=$(git describe --tags --abbrev=0 2>/dev/null) || TAG="No Tag"

echo "Hello"
echo "#define GIT_VERSION \"$(git rev-parse --short HEAD)\"" > git_version.h
echo "#define GIT_LVERSION \"$(git rev-parse HEAD)\"" >> git_version.h
echo "#define GIT_COUNT \"$(git rev-list --count HEAD)\"" >> git_version.h
echo "#define GIT_AUTHOR \"$(git log -1 --pretty=format:'%an')\"" >> git_version.h
echo "#define GIT_TAG \"${TAG}\"" >> git_version.h
echo "#define GIT_BRANCH \"$(git rev-parse --abbrev-ref HEAD)\"" >> git_version.h
