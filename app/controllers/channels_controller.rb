class ChannelsController < ApplicationController
  before_action :authenticate_user!
  before_action :generate_channels
  before_action :activate_osc

  def index
    @channels = Channel.where(user: current_user).order("id")
    @user     = current_user
  end

  private

  def generate_channels
    Channel.generate_channels(current_user)
  end

  def activate_osc
    Channel.activate_osc
  end

end
