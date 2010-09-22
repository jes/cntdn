#!/usr/bin/env python
# GTK interface to countdown letters solver
# James Stanley 2010

import subprocess
import sys

import pygtk
pygtk.require('2.0')
import gtk
import pango

def lengthcmp(s1, s2):
  return len(s2) - len(s1)

class GLetters:
  def delete_event(self, widget, event, data=None):
    return False

  def destroy(self, widget, data=None):
    gtk.main_quit()

  def solve_letters(self, widget, event, data=None):
    self.solver.stdin.write(self.inputbox.get_text().lower() + '\n')

    lines = []

    while True:
      line = self.solver.stdout.readline()
      if line == '\n':
        break
      if line == '':
        sys.exit()
      lines.append(line)

    text = ''
    for line in sorted(lines, key = lambda s: -len(s)):
      text = text + line.capitalize()
    self.wordlist.set_text(text)

  def __init__(self):
    self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
    self.inputbox = gtk.Entry()
    self.wordlist = gtk.TextBuffer()

    self.window.set_title('gletters')
    self.window.connect('delete_event', self.delete_event)
    self.window.connect('destroy', self.destroy)
    self.window.resize(200, 300)
    self.window.set_position(gtk.WIN_POS_CENTER)

    self.inputbox.connect("key_release_event", self.solve_letters)

    textview = gtk.TextView()
    textview.set_buffer(self.wordlist)
    textview.set_editable(False)
    textview.modify_font(pango.FontDescription('Monospace'))

    box = gtk.VBox(False, 0)
    box.pack_start(self.inputbox, False, True, 0)
    sw = gtk.ScrolledWindow()
    sw.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)
    sw.add(textview)
    box.pack_start(sw, True, True, 0)
    self.window.add(box)

    self.window.show_all()

    self.solver = subprocess.Popen(['letters', '-I', '-m0'], \
                  stdin=subprocess.PIPE, stdout=subprocess.PIPE)

  def main(self):
    gtk.main()

if __name__ == '__main__':
  gletters = GLetters()
  gletters.main()
