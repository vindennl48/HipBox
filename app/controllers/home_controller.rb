class HomeController < ApplicationController
  before_action :authenticate_user!

  def index
    @variables = Variable.where(user_id: current_user.id).order(:id)
  end

  def admin
    if current_user.name != "mitch"
      redirect_to root_path
    end
  end

  def upload_files
    if current_user.name != "mitch"
      redirect_to root_path
    end

    respond_to do |format|
      format.js
    end
    UploadRecordJob.perform_later
  end

  def delete_files
    if current_user.name != "mitch"
      redirect_to root_path
    end

    respond_to do |format|
      format.js
    end
    DeleteFilesJob.perform_later
  end

end
