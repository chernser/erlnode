srcdir = '.'
blddir = 'build'
VERSION = '0.0.1'

def set_options(opt):
  opt.tool_options('compiler_cxx')

def configure(conf):
  conf.check_tool('compiler_cxx')
  conf.check_tool('node_addon')
  conf.env.append_unique('CXXFLAGS', ['-Wall', '-O3'])

def build(bld):
  obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
  obj.cxxflags = ["-I../deps", "-I/usr/local/lib/erlang/lib/erl_interface-3.7.5/include", "-g", "-Wall"]
  obj.target = "erlnode"
  obj.source = ["erlnode.cc", "module.cc"]
  
