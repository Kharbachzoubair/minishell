#!/bin/bash

./minishell << EOF
echo foo | wc -w
ls | grep minishell
echo Hello World | tr a-z A-Z | rev
EOF
