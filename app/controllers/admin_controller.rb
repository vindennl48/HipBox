class AdminController < ApplicationController
  before_action :authenticate_user!
  before_action :is_admin

  def index
  end

  private

  def is_admin
    if current_user != User.find_by(name: "Mitch")
      redirect_to channels_path
    end
  end

end
