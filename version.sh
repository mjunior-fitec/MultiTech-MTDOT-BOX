#!/bin/bash

VERSION=`git describe`

echo -e "#ifndef __VERSION_H__\r\n#define __VERSION_H__\r\n\r\n#define MTDOT_BOX_VERSION \"$VERSION\"\r\n\r\n#endif" > dotbox_version.h
