/*
 ============================================================================
 Author	    : Dmitry Moskalchuk
 Version	: 1.5
 Copyright  : Copyright (C) 2008 Rhomobile. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ============================================================================
 */
package com.rhomobile.rhodes;

public class WebView {
	
	private static final String TAG = "WebView";
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}
	
	private static class NavigateTask implements Runnable {
		private String url;
		private int index;
		
		public NavigateTask(String u, int i) {
			url = u;
			index = i;
		}
		
		public void run() {
			Rhodes r = RhodesInstance.getInstance();
			r.getMainView().navigate(url, index);
		}
	};
	
	private static class RefreshTask implements Runnable {
		private int index;
		
		public RefreshTask(int i) {
			index = i;
		}
		
		public void run() {
			RhodesInstance.getInstance().getMainView().reload(index);
		}
	};
	
	private static class LocationTask implements Runnable {
		private int index;
		private StringBuffer loc;
		
		public LocationTask(int i, StringBuffer l) {
			index = i;
			loc = l;
		}
		
		public void run() {
			loc.delete(0, -1);
			loc.append(RhodesInstance.getInstance().getMainView().currentLocation(index));
		} 
	};
	
	private static class IntHolder {
		public int value;
	};
	
	private static class ActiveTabTask implements Runnable {
		private IntHolder ret;
		
		public ActiveTabTask(IntHolder r) {
			ret = r;
		}
		
		public void run() {
			ret.value = RhodesInstance.getInstance().getMainView().activeTab();
		}
	};

	public static void navigate(String url, int index) {
		try {
			Rhodes.performOnUiThread(new NavigateTask(url, index), false);
		}
		catch (Exception e) {
			reportFail("navigate", e);
		}
	}
	
	public static void refresh(int index) {
		try {
			Rhodes.performOnUiThread(new RefreshTask(index), false);
		}
		catch (Exception e) {
			reportFail("refresh", e);
		}
	}
	
	public static String currentLocation(int index) {
		try {
			StringBuffer loc = new StringBuffer();
			Rhodes.performOnUiThread(new LocationTask(index, loc), true);
			return loc.toString();
		}
		catch (Exception e) {
			reportFail("currentLocation", e);
		}
		
		return "";
	}
	
	public static int activeTab() {
		try {
			IntHolder v = new IntHolder();
			Rhodes.performOnUiThread(new ActiveTabTask(v), true);
			return v.value;
		}
		catch (Exception e) {
			reportFail("activeTab", e);
		}
		
		return 0;
	}
	
	public static String executeJs(String js, int index) {
		try {
			Rhodes.performOnUiThread(new NavigateTask("javascript:" + js, index), false);
		}
		catch (Exception e) {
			reportFail("executeJs", e);
		}
		
		return "";
	}
}
