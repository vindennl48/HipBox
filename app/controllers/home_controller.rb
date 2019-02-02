class HomeController < ApplicationController
  before_action :authenticate_user!

  def index
    @variables = Variable.where(user_id: current_user.id).order(:id)
  end
end
