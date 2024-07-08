#!/usr/bin/env ruby

require "rexml"

COMMAND_NAME = ARGV[0]
XBEL_FILE_PATH = ARGV[1]

xbel_doc = File.open(XBEL_FILE_PATH, "r") { |file| REXML::Document.new(file) }
xbel_doc.get_elements("/xbel//bookmark").each do |bookmark|
  bookmark_uri = bookmark["href"].strip
  spawn("#{COMMAND_NAME} \"#{bookmark_uri}\"")
end
