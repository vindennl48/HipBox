class ChannelsController < ApplicationController
  before_action :authenticate_user!
  before_action :generate_channels

  def index
    @channels  = Channel.where(user: current_user)
    @user_name = current_user.name
  end

  private

  def generate_channels
    Channel.generate_channels(current_user)
  end

end
