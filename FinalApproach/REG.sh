#!/bin/bash
cp AST.service /lib/systemd/system/AST.service
ln -s /lib/systemd/system/AST.service /etc/systemd/system/multi-user.target.wants/AST.service
