class ChannelsController < ApplicationController
  before_action :authenticate_user!

  def index
    @channels = Channel.where(user: current_user)
  end
end
