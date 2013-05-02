#! /usr/bin/env python
# encoding: utf-8

VERSION='0.0.1'
APPNAME='cpp-generator'


top = '.'
out = 'build'


def options(opt):
  opt.load('compiler_cxx flex bison')


def configure(conf):
  conf.load('compiler_cxx flex bison')


def build(bld):
  bld.recurse('include lib tools')
