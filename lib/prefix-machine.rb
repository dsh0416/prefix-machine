require 'prefix_machine_ext'
require './prefix-machine/version'

class PrefixMachine
  attr_reader :count

  def guard!(str)
    raise NoMatchingPatternError.new('No Such Patterns') unless str =~ /[0-9a-z.]*/
  end

  def insert(rule)
    guard!(rule)
    trie_insert(rule)
  end

  def match(str)
    guard!(str)
    trie_match(str)
  end

  alias_method :<<, :insert
end
