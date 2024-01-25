python
# https://sourceware.org/gdb/onlinedocs/gdb/Python-API.html#Python-API
# https://sourceware.org/gdb/onlinedocs/gdb/Functions-In-Python.html
class Greet (gdb.Function):
  """Return string to greet someone.
  Takes a name as argument."""
  def __init__ (self):
    super (Greet, self).__init__ ("greet")
  def invoke (self, name):
    return "Hello, %s!" % name.string ()
Greet() # instantiate; then call in gdb: p $greet("myname")

#(gdb) python gdb.execute("p 22")
#$2 = 22

class Strp (gdb.Function):
  """Get a representation of gdb's printout as string
  Argument: the gdb command as string."""
  def __init__ (self):
    super (Strp, self).__init__ ("strp")
  def invoke (self, incmd):
    incmds = str(incmd)
    #print("is",incmds)
    # strip quotes
    if (incmds[0] == incmds[-1]) and incmds.startswith(("'", '"')):
      incmds = incmds[1:-1]
    rets=""
    try:
      rets += gdb.execute(incmds, from_tty=True, to_string=True)
    except Exception as e:
      rets += "Exception: {0}".format(e)
    #print("ret",ret)
    return rets
Strp() # instantiate; then call in gdb: p $strp("p 22")
# quotes end up in arg string, so by default getting Python Exception <class 'gdb.error'> Undefined command: "".  Try "help".: ... but then, if call WITHOUT quotes p $strp(p aa), gdb may interpret "p" as symbol (which doesn't exist) before it is sent as arg to python; therefore, use quotes, then strip them
end