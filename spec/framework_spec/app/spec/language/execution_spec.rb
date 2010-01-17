require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../spec_helper'

describe "``" do
  it "returns the output of the executed sub-process" do
    ip = 'world'
    `echo disc #{ip}`.should == "disc world\n"
  end
end

describe "%x" do
  it "is the same as ``" do
    ip = 'world'
    %x(echo disc #{ip}).should == "disc world\n"
  end
end