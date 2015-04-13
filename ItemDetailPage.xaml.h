//
// ItemDetailPage.xaml.h
// Declaration of the ItemDetailPage class
//

#pragma once

#include "ItemDetailPage.g.h"
#include "ofXamlAppMain.h"

using namespace ofXamlApp;

namespace Somability_UI
{
	/// <summary>
	/// A page that displays details for a single item within a group while allowing gestures to
	/// flip through other items belonging to the same group.
	/// </summary>
	[Windows::UI::Xaml::Data::Bindable]
	public ref class ItemDetailPage sealed
	{
	public:
		ItemDetailPage();
		/// <summary>
		/// This can be changed to a strongly typed view model.
		/// </summary>
		property Windows::Foundation::Collections::IObservableMap<Platform::String^, Platform::Object^>^ DefaultViewModel
		{
			Windows::Foundation::Collections::IObservableMap<Platform::String^, Platform::Object^>^  get();
		}
		/// <summary>
		/// NavigationHelper is used on each page to aid in navigation and 
		/// process lifetime management
		/// </summary>
		property Common::NavigationHelper^ NavigationHelper
		{
			Common::NavigationHelper^ get();
		}

		void OnPreviousColorPressed(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnNextColorPressed(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		void SaveInternalState(Windows::Foundation::Collections::IPropertySet^ state);
		void LoadInternalState(Windows::Foundation::Collections::IPropertySet^ state);

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
		virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

	private:
		void LoadState(Platform::Object^ sender, Common::LoadStateEventArgs^ e);

		static Windows::UI::Xaml::DependencyProperty^ _defaultViewModelProperty;
		static Windows::UI::Xaml::DependencyProperty^ _navigationHelperProperty;

		void OnPointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ args);
		void OnPointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ args);
		void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ arg);
		void OnRendering(Object^ sender, Object^ args);

		Windows::Foundation::EventRegistrationToken m_eventToken;

		ofXamlAppMain^ m_renderer;
		bool m_renderNeeded;

		Windows::Foundation::Point m_lastPoint;
		bool m_lastPointValid;

		//BasicTimer^ m_timer;
	};
}
