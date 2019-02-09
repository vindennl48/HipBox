Rails.application.routes.draw do
  devise_for :users
  resources :networks

  namespace :api do
    namespace :v1 do
      resources :users, only: [:index, :update]
      resources :variables, only: [:update]
      put 'variables', to: 'variables#index'
    end
  end

  get 'admin', to: 'home#admin'
  namespace :home do
    get  'index',        to: 'index'
    post 'upload_files', to: 'upload_files'
  end

  root "home#index"
end
