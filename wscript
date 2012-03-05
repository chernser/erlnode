srcdir = '.'
blddir = 'build'
VERSION = '0.0.1'

erl_interface_dir = "/usr/local/lib/erlang/lib/erl_interface-3.7.6/"
erl_interface_libs = ["-lerl_interface", "-lei", "-lnsl"]

def set_options(opt):
  opt.tool_options('compiler_cxx')

def configure(conf):
  conf.check_tool('compiler_cxx')
  conf.check_tool('node_addon')
  conf.env.append_unique('CXXFLAGS', ['-Wall', '-O3', '-g'])

def build(bld):
  obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
  bld.env.append_value('LINKFLAGS', ["-L" + erl_interface_dir + "lib"])
  bld.env.append_value('LINKFLAGS', erl_interface_libs) 
  obj.cxxflags = ["-I../deps", "-I" + erl_interface_dir + "include", "-g", "-Wall"]
  obj.target = "erlnode"
  obj.source = ["erlnode.cc", "module.cc"]
  
