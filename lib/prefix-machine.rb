require 'prefix_machine_ext'
require './prefix-machine/version'

class PrefixMachine
  attr_reader :count

  def guard!(str)
    raise NoMatchingPatternError.new('No Such Patterns') unless str =~ /[0-9a-z.]*/
  end

  def insert(rule)
    s = rule.downcase
    guard!(s)
    trie_insert(s)
  end

  def match(str)
    s = str.downcase
    guard!(s)
    trie_match(s)
  end

  alias_method :<<, :insert
end
