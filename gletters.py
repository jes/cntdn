#!/usr/bin/env python
# GTK interface to countdown letters solver
# James Stanley 2010

import subprocess

import pygtk
pygtk.require('2.0')
import gtk

class GLetters:
  def delete_event(self, widget, event, data=None):
    return False

  def destroy(self, widget, data=None):
    gtk.main_quit()

  def solve_letters(self, widget, event, data=None):
    p = subprocess.Popen(['./letters', self.inputbox.get_text()], \
        stdout=subprocess.PIPE)
    text = p.communicate()[0]
    self.wordlist.set_text(text)

  def __init__(self):
    self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
    self.inputbox = gtk.Entry()
    self.wordlist = gtk.TextBuffer()

    self.window.set_title("gletters")
    self.window.connect("delete_event", self.delete_event)
    self.window.connect("destroy", self.destroy)

    self.inputbox.connect("key_release_event", self.solve_letters)

    textview = gtk.TextView()
    textview.set_buffer(self.wordlist)
    textview.set_editable(False)

    box = gtk.VBox(False, 0)
    box.pack_start(self.inputbox, False, True, 0)
    sw = gtk.ScrolledWindow()
    sw.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)
    sw.add(textview)
    box.pack_start(sw, True, True, 0)
    self.window.add(box)

    self.window.show_all()

  def main(self):
    gtk.main()

if __name__ == "__main__":
  gletters = GLetters()
  gletters.main()
