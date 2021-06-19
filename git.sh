#!/bin/bash

set -e

git add .
git commit -m $1

if [[ $2 == p ]] 
then
git push
git push official master:automate-input
fi
