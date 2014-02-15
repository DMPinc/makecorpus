#!/usr/bin/env ruby
require 'nokogiri'
require 'open-uri'
require 'net/http'

html = Nokogiri::HTML(open('http://piano.s20.xrea.com/midi/main.html'))

html.css('a').each do |anchor|
  href = anchor[:href]
  if href != nil && href.end_with?(".mid") then
    download_url = "http://piano.s20.xrea.com/midi/" + href
    # p download_url
    `wget #{download_url}`
  end
end
